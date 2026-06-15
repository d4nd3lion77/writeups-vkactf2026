const http = require("http");
const crypto = require("crypto");
const fs = require("fs");
const path = require("path");
const express = require("express");
const session = require("express-session");
const SQLiteStoreFactory = require("connect-sqlite3");
const helmet = require("helmet");
const bcrypt = require("bcryptjs");
const { WebSocketServer } = require("ws");

const config = require("./config");
const store = require("./db");
const { enqueueVisit, canVisitUrl } = require("./support-bot");

store.seed();

const app = express();
const SQLiteStore = SQLiteStoreFactory(session);
const server = http.createServer(app);

app.set("trust proxy", 1);

const sessionParser = session({
  store: new SQLiteStore({ db: "sessions.db", dir: config.DATA_DIR }),
  name: "deskline.sid",
  secret: config.SESSION_SECRET,
  resave: false,
  saveUninitialized: false,
  cookie: {
    httpOnly: true,
    path: "/",
    sameSite: config.COOKIE_SAMESITE,
    secure: config.COOKIE_SECURE,
    maxAge: 1000 * 60 * 60 * 8,
  },
});

app.use(helmet({
  contentSecurityPolicy: {
    useDefaults: true,
    directives: {
      "default-src": ["'self'"],
      "script-src": ["'self'"],
      "style-src": ["'self'"],
      "img-src": ["'self'", "data:"],
      "connect-src": ["'self'", "ws:", "wss:"],
      "object-src": ["'none'"],
      "base-uri": ["'none'"],
      "frame-ancestors": ["'none'"],
    },
  },
}));
app.use(express.json({ limit: "32kb" }));
app.use(express.urlencoded({ extended: false }));
app.use(sessionParser);
app.get("/favicon.ico", (req, res) => res.status(204).end());
app.use(express.static(path.join(__dirname, "..", "public")));

function csrf(req, res, next) {
  if (!req.session.csrf) req.session.csrf = crypto.randomBytes(16).toString("hex");
  if (["GET", "HEAD", "OPTIONS"].includes(req.method)) return next();
  const sent = req.get("x-csrf-token") || req.body._csrf;
  if (sent !== req.session.csrf) return res.status(403).json({ error: "bad csrf token" });
  next();
}

function currentUser(req) {
  if (!req.session.userId) return null;
  return store.getUserById(req.session.userId);
}

function requireUser(req, res, next) {
  const user = currentUser(req);
  if (!user) return res.status(401).json({ error: "login required" });
  req.user = user;
  next();
}

function validUsername(username) {
  return /^[a-zA-Z0-9_]{3,24}$/.test(String(username || ""));
}

app.get("/", (req, res) => {
  res.sendFile(path.join(__dirname, "..", "views", "index.html"));
});

app.get("/desk/thread", (req, res) => {
  res.sendFile(path.join(__dirname, "..", "views", "desk-thread.html"));
});

app.get(["/chats", "/friends", "/profile", "/chat/:username"], (req, res) => {
  res.sendFile(path.join(__dirname, "..", "views", "index.html"));
});

app.use("/api", csrf);

app.get("/api/me", (req, res) => {
  const user = currentUser(req);
  res.json({
    user: store.publicUser(user),
    csrf: req.session.csrf,
    friends: user ? store.listFriends(user.id) : [],
    chats: user ? store.listChats(user.id) : [],
  });
});

app.post("/api/register", (req, res) => {
  const { username, password } = req.body;
  if (!validUsername(username)) return res.status(400).json({ error: "invalid username" });
  if (String(password || "").length < 6) return res.status(400).json({ error: "short password" });
  if (store.getUserByName(username)) return res.status(409).json({ error: "username is busy" });

  const user = store.createUser(username, password);
  const support = store.getUserByName("support");
  store.setFriends(user.id, support.id);

  req.session.userId = user.id;
  req.session.csrf = crypto.randomBytes(16).toString("hex");
  res.json({ ok: true, user: store.publicUser(user), csrf: req.session.csrf });
});

app.post("/api/login", (req, res) => {
  const user = store.getUserByName(req.body.username);
  if (!user || !bcrypt.compareSync(String(req.body.password || ""), user.password_hash)) {
    return res.status(401).json({ error: "invalid credentials" });
  }
  req.session.userId = user.id;
  req.session.csrf = crypto.randomBytes(16).toString("hex");
  res.json({ ok: true, user: store.publicUser(user), csrf: req.session.csrf });
});

app.post("/api/logout", requireUser, (req, res) => {
  req.session.destroy(() => res.json({ ok: true }));
});

app.get("/api/users/search", requireUser, (req, res) => {
  res.json({ users: store.searchUsers(req.query.q, req.user.id).map(store.publicUser) });
});

app.get("/api/users/:username", requireUser, (req, res) => {
  const user = store.getUserByName(req.params.username);
  if (!user) return res.status(404).json({ error: "not found" });
  res.json({ user: store.publicUser(user) });
});

app.post("/api/profile", requireUser, (req, res) => {
  const firstName = String(req.body.first_name || "").slice(0, 60);
  const lastName = String(req.body.last_name || "").slice(0, 60);
  const position = String(req.body.position || "").slice(0, 80);
  const location = String(req.body.location || "").slice(0, 80);
  const hasStatus = Object.prototype.hasOwnProperty.call(req.body, "bio");
  const bio = hasStatus ? String(req.body.bio || "").slice(0, 240) : req.user.bio;
  const displayName = [firstName, lastName].filter(Boolean).join(" ") || req.user.username;
  store.db.prepare(`
    UPDATE users
    SET first_name = ?, last_name = ?, position = ?, location = ?, bio = ?, display_name = ?
    WHERE id = ?
  `).run(firstName, lastName, position, location, bio, displayName, req.user.id);
  res.json({ ok: true, user: store.publicUser(store.getUserById(req.user.id)) });
});

app.post("/api/friends/request", requireUser, (req, res) => {
  const target = store.getUserByName(req.body.username);
  if (!target) return res.status(404).json({ error: "User was not found." });
  if (target.id === req.user.id) return res.status(400).json({ error: "You cannot add yourself." });
  if (target.role === "admin") return res.status(403).json({ error: "The admin inbox does not accept friend requests." });
  store.setFriends(req.user.id, target.id);
  res.json({ ok: true, status: "accepted" });
});

app.post("/api/friends/accept", requireUser, (req, res) => {
  const target = store.getUserByName(req.body.username);
  if (!target) return res.status(404).json({ error: "not found" });
  const incoming = store.q.friendship.get(target.id, req.user.id);
  if (!incoming || incoming.status !== "pending") return res.status(404).json({ error: "request not found" });
  store.setFriends(req.user.id, target.id);
  res.json({ ok: true });
});

const socketsByUser = new Map();

function rememberSocket(userId, ws) {
  if (!socketsByUser.has(userId)) socketsByUser.set(userId, new Set());
  socketsByUser.get(userId).add(ws);
  ws.on("close", () => socketsByUser.get(userId)?.delete(ws));
}

function sendToUser(userId, payload) {
  const data = JSON.stringify(payload);
  for (const socket of socketsByUser.get(userId) || []) {
    if (socket.readyState === socket.OPEN) socket.send(data);
  }
}

function broadcastMessage(message) {
  const chat = store.q.chatById.get(message.chat_id);
  if (!chat) return;
  const payload = {
    type: "message",
    message: {
      id: message.id,
      chat_id: message.chat_id,
      body: message.body,
      sender: message.sender_username,
      created_at: message.created_at,
    },
  };
  sendToUser(chat.user_low, payload);
  sendToUser(chat.user_high, payload);
}

function extractUrls(text) {
  return String(text || "").match(/https?:\/\/[^\s<>"']{3,}/gi) || [];
}

function readFlag() {
  try {
    return fs.readFileSync(config.FLAG_PATH, "utf8").trim() || "vkactf{missing_flag}";
  } catch {
    return "vkactf{missing_flag}";
  }
}

function sendSystemMessage(from, to, body) {
  if (!store.canMessage(from, to)) return null;
  const chat = store.getOrCreateChat(from.id, to.id);
  const message = store.addMessage(chat.id, from.id, body);
  broadcastMessage(message);
  handleAutomation(message);
  return message;
}

function handleAutomation(message) {
  const chat = store.q.chatById.get(message.chat_id);
  const sender = store.getUserById(message.sender_id);
  const peer = store.getChatPeer(chat, sender.id);
  if (!sender || !peer) return;

  if (peer.role === "support" && sender.role === "user") {
    const support = peer;
    const admin = store.getUserByName("admin");
    const body = message.body.trim();

    if (/^\/report\b/i.test(body)) {
      const subject = body.replace(/^\/report\s*/i, "").slice(0, 500) || "No details";
      const reviewChat = store.getOrCreateChat(support.id, admin.id);
      const ticket = store.createTicket({
        customerId: sender.id,
        customerChatId: chat.id,
        supportId: support.id,
        reviewerId: admin.id,
        reviewChatId: reviewChat.id,
        subject,
        lastNote: "Waiting for duty inbox review.",
      });
      sendSystemMessage(support, admin, `Report ${ticket.ticket_uid} from @${sender.username}: ${subject}`);
      sendSystemMessage(support, sender, `Report ${ticket.ticket_uid} has been passed to the duty inbox.`);
      return;
    }

    const urls = extractUrls(body).filter(canVisitUrl);
    if (urls.length) {
      sendSystemMessage(support, sender, "I will check the link and get back to you.");
      for (const url of urls.slice(0, 2)) enqueueVisit(url);
      return;
    }

    sendSystemMessage(support, sender, "Thanks, the support desk received your message.");
    return;
  }

  if (peer.role === "admin" && sender.role === "support") {
    const admin = peer;
    const support = sender;
    if (/(^|\s)(flag|флаг)(\s|$)/i.test(message.body)) {
      sendSystemMessage(admin, support, readFlag());
    } else {
      const reply = "Ticket received. The duty inbox is watching this thread.";
      store.updateLatestTicketNote(support.id, chat.id, reply);
      sendSystemMessage(admin, support, reply);
    }
  }
}

const wss = new WebSocketServer({ noServer: true });

server.on("upgrade", (req, socket, head) => {
  if (!req.url.startsWith("/ws")) return socket.destroy();
  sessionParser(req, {}, () => {
    const user = req.session?.userId ? store.getUserById(req.session.userId) : null;
    if (!user) {
      console.error("ws rejected: no valid session", {
        origin: req.headers.origin || "",
        hasCookie: Boolean(req.headers.cookie),
      });
      return socket.destroy();
    }
    console.log("ws accepted", {
      user: user.username,
      origin: req.headers.origin || "",
    });
    req.user = user;
    wss.handleUpgrade(req, socket, head, ws => wss.emit("connection", ws, req));
  });
});

function wsError(ws, message) {
  ws.send(JSON.stringify({ type: "error", error: message }));
}

wss.on("connection", (ws, req) => {
  const user = req.user;
  rememberSocket(user.id, ws);
  ws.send(JSON.stringify({ type: "hello", user: store.publicUser(user) }));

  ws.on("message", raw => {
    let msg;
    try {
      msg = JSON.parse(raw.toString());
    } catch {
      return wsError(ws, "bad json");
    }

    if (msg.op === "sync_inbox" || msg.op === "sync_unread") {
      if (user.role !== "support") return wsError(ws, "support inbox unavailable");
      if (!msg.ticket_uid) return wsError(ws, "ticket uid required");
      const ticket = store.supportTicketByUid(user.id, msg.ticket_uid);
      return ws.send(JSON.stringify({ type: "inbox_sync", ticket: store.publicTicket(ticket) }));
    }

    if (msg.op === "watch_ticket_thread") {
      if (user.role !== "support") return wsError(ws, "support inbox unavailable");
      const ticket = store.supportTicketByUid(user.id, msg.ticket_uid);
      if (!ticket || ticket.review_thread !== msg.thread) return wsError(ws, "ticket thread not found");
      return ws.send(JSON.stringify({
        type: "thread_event",
        ticket_uid: ticket.ticket_uid,
        thread: ticket.review_thread,
        event: "review_attached",
        peer: ticket.reviewer,
        chat_id: ticket.review_chat_id,
      }));
    }

    if (msg.op === "open_dm") {
      const peer = store.getUserByName(msg.username);
      if (!store.canMessage(user, peer)) return wsError(ws, "conversation unavailable");
      const chat = store.getOrCreateChat(user.id, peer.id);
      store.markRead(chat.id, user.id);
      return ws.send(JSON.stringify({
        type: "dm_opened",
        chat_id: chat.id,
        peer: store.publicUser(peer),
        messages: store.getMessages(chat.id, user.id),
      }));
    }

    if (msg.op === "read_chat") {
      const chat = store.q.chatById.get(Number(msg.chat_id));
      if (!store.getChatPeer(chat, user.id)) return wsError(ws, "chat not found");
      store.markRead(chat.id, user.id);
      return ws.send(JSON.stringify({ type: "read_done", chat_id: chat.id }));
    }

    if (msg.op === "send_message") {
      const chat = store.q.chatById.get(Number(msg.chat_id));
      const peer = store.getChatPeer(chat, user.id);
      if (!peer || !store.canMessage(user, peer)) return wsError(ws, "conversation unavailable");
      const message = store.addMessage(chat.id, user.id, msg.body);
      broadcastMessage(message);
      handleAutomation(message);
      return;
    }

    wsError(ws, "unknown operation");
  });
});

server.listen(config.PORT, () => {
  console.log(`Deskline listening on ${config.BASE_URL}`);
});
