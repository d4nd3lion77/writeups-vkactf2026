const fs = require("fs");
const crypto = require("crypto");
const Database = require("better-sqlite3");
const bcrypt = require("bcryptjs");
const config = require("./config");

fs.mkdirSync(config.DATA_DIR, { recursive: true });

const db = new Database(config.DB_PATH);
db.pragma("journal_mode = WAL");
db.pragma("foreign_keys = ON");

db.exec(`
  CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE COLLATE NOCASE,
    password_hash TEXT NOT NULL,
    role TEXT NOT NULL DEFAULT 'user',
    display_name TEXT NOT NULL,
    first_name TEXT NOT NULL DEFAULT '',
    last_name TEXT NOT NULL DEFAULT '',
    position TEXT NOT NULL DEFAULT '',
    location TEXT NOT NULL DEFAULT '',
    bio TEXT NOT NULL DEFAULT '',
    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
  );

  CREATE TABLE IF NOT EXISTS friendships (
    user_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    friend_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    status TEXT NOT NULL,
    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (user_id, friend_id),
    CHECK (user_id <> friend_id)
  );

  CREATE TABLE IF NOT EXISTS chats (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_low INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    user_high INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
    UNIQUE (user_low, user_high),
    CHECK (user_low <> user_high)
  );

  CREATE TABLE IF NOT EXISTS messages (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    chat_id INTEGER NOT NULL REFERENCES chats(id) ON DELETE CASCADE,
    sender_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    body TEXT NOT NULL,
    read_by_recipient INTEGER NOT NULL DEFAULT 0,
    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
  );

  CREATE TABLE IF NOT EXISTS tickets (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    ticket_uid TEXT NOT NULL DEFAULT '',
    review_thread TEXT NOT NULL DEFAULT '',
    customer_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    customer_chat_id INTEGER NOT NULL REFERENCES chats(id) ON DELETE CASCADE,
    support_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    reviewer_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    review_chat_id INTEGER NOT NULL REFERENCES chats(id) ON DELETE CASCADE,
    status TEXT NOT NULL DEFAULT 'waiting_review',
    subject TEXT NOT NULL,
    last_note TEXT NOT NULL DEFAULT '',
    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
    expires_at TEXT NOT NULL
  );

  CREATE INDEX IF NOT EXISTS idx_tickets_support_queue
  ON tickets (support_id, status, expires_at);

  CREATE INDEX IF NOT EXISTS idx_tickets_customer
  ON tickets (customer_id, created_at);
`);

function ensureColumn(table, column, definition) {
  const columns = db.prepare(`PRAGMA table_info(${table})`).all().map(row => row.name);
  if (!columns.includes(column)) {
    db.exec(`ALTER TABLE ${table} ADD COLUMN ${column} ${definition}`);
  }
}

ensureColumn("users", "first_name", "TEXT NOT NULL DEFAULT ''");
ensureColumn("users", "last_name", "TEXT NOT NULL DEFAULT ''");
ensureColumn("users", "position", "TEXT NOT NULL DEFAULT ''");
ensureColumn("users", "location", "TEXT NOT NULL DEFAULT ''");
ensureColumn("tickets", "ticket_uid", "TEXT NOT NULL DEFAULT ''");
ensureColumn("tickets", "review_thread", "TEXT NOT NULL DEFAULT ''");

db.exec(`
  UPDATE tickets
  SET ticket_uid = 'tkt_legacy_' || id
  WHERE ticket_uid = '';

  UPDATE tickets
  SET review_thread = 'rev_legacy_' || id
  WHERE review_thread = '';

  CREATE UNIQUE INDEX IF NOT EXISTS idx_tickets_uid
  ON tickets (ticket_uid);

  CREATE UNIQUE INDEX IF NOT EXISTS idx_tickets_review_thread
  ON tickets (review_thread);
`);

const q = {
  userByName: db.prepare("SELECT * FROM users WHERE username = ?"),
  userById: db.prepare("SELECT * FROM users WHERE id = ?"),
  createUser: db.prepare(`
    INSERT INTO users (username, password_hash, role, display_name, bio)
    VALUES (?, ?, ?, ?, ?)
  `),
  updateServiceUser: db.prepare(`
    UPDATE users
    SET password_hash = ?, role = ?, display_name = ?, bio = ?
    WHERE id = ?
  `),
  upsertFriendship: db.prepare(`
    INSERT INTO friendships (user_id, friend_id, status)
    VALUES (?, ?, ?)
    ON CONFLICT(user_id, friend_id)
    DO UPDATE SET status = excluded.status, updated_at = CURRENT_TIMESTAMP
  `),
  friendship: db.prepare(`
    SELECT * FROM friendships
    WHERE user_id = ? AND friend_id = ?
  `),
  ensureChat: db.prepare(`
    INSERT INTO chats (user_low, user_high)
    VALUES (?, ?)
    ON CONFLICT(user_low, user_high) DO NOTHING
  `),
  chatByPair: db.prepare(`
    SELECT * FROM chats WHERE user_low = ? AND user_high = ?
  `),
  chatById: db.prepare("SELECT * FROM chats WHERE id = ?"),
  insertMessage: db.prepare(`
    INSERT INTO messages (chat_id, sender_id, body)
    VALUES (?, ?, ?)
  `),
  markRead: db.prepare(`
    UPDATE messages
    SET read_by_recipient = 1
    WHERE chat_id = ? AND sender_id <> ?
  `),
  createTicket: db.prepare(`
    INSERT INTO tickets (
      ticket_uid, review_thread,
      customer_id, customer_chat_id, support_id, reviewer_id, review_chat_id,
      status, subject, last_note, expires_at
    )
    VALUES (?, ?, ?, ?, ?, ?, ?, 'waiting_review', ?, ?, datetime('now', '+20 minutes'))
  `),
  ticketById: db.prepare("SELECT * FROM tickets WHERE id = ?"),
  updateLatestTicketNote: db.prepare(`
    UPDATE tickets
    SET last_note = ?, updated_at = CURRENT_TIMESTAMP
    WHERE id = (
      SELECT id
      FROM tickets
      WHERE support_id = ?
        AND review_chat_id = ?
        AND status = 'waiting_review'
        AND datetime(expires_at) > datetime('now')
      ORDER BY id DESC
      LIMIT 1
    )
  `),
  supportTicketByUid: db.prepare(`
    SELECT
      t.*,
      customer.username AS customer,
      customer.display_name AS customer_name,
      reviewer.username AS reviewer
    FROM tickets t
    JOIN users customer ON customer.id = t.customer_id
    JOIN users reviewer ON reviewer.id = t.reviewer_id
    WHERE t.support_id = ?
      AND t.ticket_uid = ?
      AND t.status = 'waiting_review'
      AND datetime(t.expires_at) > datetime('now')
  `),
};

function normalizeUsername(username) {
  return String(username || "").trim();
}

function publicUser(user) {
  if (!user) return null;
  return {
    id: user.id,
    username: user.username,
    role: user.role,
    display_name: user.display_name,
    first_name: user.first_name,
    last_name: user.last_name,
    position: user.position,
    location: user.location,
    bio: user.bio,
  };
}

function getUserByName(username) {
  return q.userByName.get(normalizeUsername(username));
}

function getUserById(id) {
  return q.userById.get(id);
}

function createUser(username, password, role = "user", displayName = null, bio = "") {
  const clean = normalizeUsername(username);
  const passwordHash = bcrypt.hashSync(password, 10);
  const info = q.createUser.run(clean, passwordHash, role, displayName || clean, bio);
  return getUserById(info.lastInsertRowid);
}

function syncServiceUser(username, password, role, displayName, bio) {
  const clean = normalizeUsername(username);
  const existing = getUserByName(clean);
  if (!existing) return createUser(clean, password, role, displayName, bio);

  const passwordHash = bcrypt.hashSync(String(password), 10);
  q.updateServiceUser.run(passwordHash, role, displayName || clean, bio, existing.id);
  return getUserById(existing.id);
}

function areFriends(a, b) {
  const row = q.friendship.get(a, b);
  return row && row.status === "accepted";
}

function setFriends(a, b) {
  q.upsertFriendship.run(a, b, "accepted");
  q.upsertFriendship.run(b, a, "accepted");
  return getOrCreateChat(a, b);
}

function requestFriend(a, b) {
  q.upsertFriendship.run(a, b, "pending");
}

function getOrCreateChat(a, b) {
  const low = Math.min(a, b);
  const high = Math.max(a, b);
  q.ensureChat.run(low, high);
  return q.chatByPair.get(low, high);
}

function getChatPeer(chat, userId) {
  if (!chat) return null;
  if (chat.user_low === userId) return getUserById(chat.user_high);
  if (chat.user_high === userId) return getUserById(chat.user_low);
  return null;
}

function canMessage(fromUser, toUser) {
  if (!fromUser || !toUser || fromUser.id === toUser.id) return false;
  if (toUser.role === "admin" && fromUser.role !== "support") return false;
  if (fromUser.role === "admin" && toUser.role !== "support") return false;
  return areFriends(fromUser.id, toUser.id);
}

function addMessage(chatId, senderId, body) {
  const text = String(body || "").slice(0, 2000);
  const info = q.insertMessage.run(chatId, senderId, text);
  return db.prepare(`
    SELECT m.*, u.username AS sender_username
    FROM messages m
    JOIN users u ON u.id = m.sender_id
    WHERE m.id = ?
  `).get(info.lastInsertRowid);
}

function getMessages(chatId, userId) {
  const chat = q.chatById.get(chatId);
  if (!getChatPeer(chat, userId)) return [];
  return db.prepare(`
    SELECT m.id, m.chat_id, m.body, m.created_at, u.username AS sender
    FROM messages m
    JOIN users u ON u.id = m.sender_id
    WHERE m.chat_id = ?
    ORDER BY m.id ASC
    LIMIT 100
  `).all(chatId);
}

function listChats(userId) {
  return db.prepare(`
    SELECT
      c.id AS chat_id,
      peer.username AS peer,
      peer.display_name AS peer_name,
      COALESCE(last.body, '') AS last_message,
      COALESCE(last.created_at, c.created_at) AS updated_at,
      (
        SELECT COUNT(*)
        FROM messages um
        WHERE um.chat_id = c.id
          AND um.sender_id <> ?
          AND um.read_by_recipient = 0
      ) AS unread
    FROM chats c
    JOIN users peer ON peer.id = CASE WHEN c.user_low = ? THEN c.user_high ELSE c.user_low END
    LEFT JOIN messages last ON last.id = (
      SELECT id FROM messages
      WHERE chat_id = c.id
      ORDER BY id DESC
      LIMIT 1
    )
    WHERE c.user_low = ? OR c.user_high = ?
    ORDER BY datetime(updated_at) DESC, c.id DESC
  `).all(userId, userId, userId, userId);
}

function unreadChats(userId) {
  return db.prepare(`
    SELECT
      c.id AS chat_id,
      peer.username AS peer,
      COUNT(m.id) AS unread,
      (
        SELECT body FROM messages lm
        WHERE lm.chat_id = c.id
        ORDER BY lm.id DESC
        LIMIT 1
      ) AS preview
    FROM messages m
    JOIN chats c ON c.id = m.chat_id
    JOIN users peer ON peer.id = CASE WHEN c.user_low = ? THEN c.user_high ELSE c.user_low END
    WHERE (c.user_low = ? OR c.user_high = ?)
      AND m.sender_id <> ?
      AND m.read_by_recipient = 0
    GROUP BY c.id
    ORDER BY MAX(m.id) DESC
  `).all(userId, userId, userId, userId);
}

function randomId(prefix) {
  return `${prefix}_${crypto.randomBytes(8).toString("hex")}`;
}

function createTicket({ customerId, customerChatId, supportId, reviewerId, reviewChatId, subject, lastNote }) {
  const ticketUid = randomId("tkt");
  const reviewThread = randomId("rev");
  const info = q.createTicket.run(
    ticketUid,
    reviewThread,
    customerId,
    customerChatId,
    supportId,
    reviewerId,
    reviewChatId,
    String(subject || "No details").slice(0, 500),
    String(lastNote || "").slice(0, 500),
  );
  return q.ticketById.get(info.lastInsertRowid);
}

function updateLatestTicketNote(supportId, reviewChatId, note) {
  q.updateLatestTicketNote.run(String(note || "").slice(0, 500), supportId, reviewChatId);
}

function supportInbox(supportId) {
  return db.prepare(`
    SELECT
      t.id AS ticket_id,
      t.status,
      t.subject,
      t.last_note,
      customer.username AS customer,
      customer.display_name AS customer_name,
      reviewer.username AS reviewer,
      t.created_at,
      t.updated_at,
      t.expires_at
    FROM tickets t
    JOIN users customer ON customer.id = t.customer_id
    JOIN users reviewer ON reviewer.id = t.reviewer_id
    WHERE t.support_id = ?
      AND t.status = 'waiting_review'
      AND datetime(t.expires_at) > datetime('now')
    ORDER BY datetime(t.updated_at) DESC, t.id DESC
    LIMIT 1000
  `).all(supportId);
}

function supportTicketByUid(supportId, ticketUid) {
  return q.supportTicketByUid.get(supportId, String(ticketUid || ""));
}

function publicTicket(ticket) {
  if (!ticket) return null;
  return {
    ticket_uid: ticket.ticket_uid,
    status: ticket.status,
    subject: ticket.subject,
    last_note: ticket.last_note,
    customer: ticket.customer,
    customer_name: ticket.customer_name,
    reviewer: ticket.reviewer,
    review_thread: ticket.review_thread,
    created_at: ticket.created_at,
    updated_at: ticket.updated_at,
    expires_at: ticket.expires_at,
  };
}

function listFriends(userId) {
  return db.prepare(`
    SELECT u.username, u.display_name, u.first_name, u.last_name, u.position, u.location, u.bio, f.status
    FROM friendships f
    JOIN users u ON u.id = f.friend_id
    WHERE f.user_id = ?
    ORDER BY u.username ASC
  `).all(userId);
}

function searchUsers(term, selfId) {
  const like = `%${String(term || "").trim()}%`;
  return db.prepare(`
    SELECT id, username, role, display_name, first_name, last_name, position, location, bio
    FROM users
    WHERE id <> ? AND username LIKE ?
    ORDER BY username ASC
    LIMIT 20
  `).all(selfId, like);
}

function seed() {
  const admin = syncServiceUser(
    "admin",
    config.ADMIN_PASSWORD,
    "admin",
    "Deskline Admin",
    "System mailbox",
  );

  const support = syncServiceUser(
    "support",
    config.SUPPORT_PASSWORD,
    "support",
    "Support Desk",
    "Support queue",
  );

  setFriends(admin.id, support.id);
}

module.exports = {
  db,
  q,
  seed,
  publicUser,
  getUserByName,
  getUserById,
  createUser,
  listFriends,
  searchUsers,
  requestFriend,
  setFriends,
  areFriends,
  canMessage,
  getOrCreateChat,
  getChatPeer,
  addMessage,
  getMessages,
  listChats,
  unreadChats,
  createTicket,
  updateLatestTicketNote,
  supportInbox,
  supportTicketByUid,
  publicTicket,
  markRead: (chatId, userId) => q.markRead.run(chatId, userId),
};
