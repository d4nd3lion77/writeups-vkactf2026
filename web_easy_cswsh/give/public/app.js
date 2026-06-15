const state = {
  csrf: "",
  user: null,
  chats: [],
  friends: [],
  ws: null,
  activeChat: null,
  toastTimer: null,
};

const els = {
  auth: document.querySelector("#auth"),
  app: document.querySelector("#app"),
  login: document.querySelector("#login-form"),
  register: document.querySelector("#register-form"),
  authError: document.querySelector("#auth-error"),
  me: document.querySelector("#me"),
  role: document.querySelector("#role"),
  avatar: document.querySelector("#avatar"),
  logout: document.querySelector("#logout"),
  navLinks: [...document.querySelectorAll(".top-nav a")],
  pages: {
    chats: document.querySelector("#page-chats"),
    friends: document.querySelector("#page-friends"),
    profile: document.querySelector("#page-profile"),
    chat: document.querySelector("#page-chat"),
  },
  chatCount: document.querySelector("#chat-count"),
  friendCount: document.querySelector("#friend-count"),
  chatSearch: document.querySelector("#chat-search"),
  chatSearchPanel: document.querySelector("#chat-search-panel"),
  chatList: document.querySelector("#chat-list"),
  supportInbox: document.querySelector("#support-inbox"),
  ticketCount: document.querySelector("#ticket-count"),
  ticketUidInput: document.querySelector("#ticket-uid-input"),
  ticketList: document.querySelector("#ticket-list"),
  friendSearch: document.querySelector("#friend-search"),
  friendSearchPanel: document.querySelector("#friend-search-panel"),
  friendList: document.querySelector("#friend-list"),
  profileForm: document.querySelector("#profile-form"),
  firstName: document.querySelector("#first-name"),
  lastName: document.querySelector("#last-name"),
  position: document.querySelector("#position"),
  locationField: document.querySelector("#location-field"),
  statusInput: document.querySelector("#status-input"),
  currentStatus: document.querySelector("#current-status"),
  saveProfile: document.querySelector("#save-profile"),
  chatTitle: document.querySelector("#chat-title"),
  chatSubtitle: document.querySelector("#chat-subtitle"),
  messages: document.querySelector("#messages"),
  messageForm: document.querySelector("#message-form"),
  messageInput: document.querySelector("#message-input"),
  sendButton: document.querySelector("#send-button"),
  report: document.querySelector("#report"),
  toast: document.querySelector("#toast"),
};

function route() {
  const path = location.pathname.replace(/\/+$/, "") || "/";
  if (path === "/friends") return { page: "friends" };
  if (path === "/profile") return { page: "profile" };
  if (path.startsWith("/chat/")) {
    return { page: "chat", username: decodeURIComponent(path.slice("/chat/".length)) };
  }
  return { page: "chats" };
}

async function api(path, options = {}) {
  const headers = { ...(options.headers || {}) };
  if (options.body && !headers["content-type"]) headers["content-type"] = "application/json";
  if (state.csrf) headers["x-csrf-token"] = state.csrf;
  const res = await fetch(path, { ...options, headers });
  const data = await res.json().catch(() => ({}));
  if (!res.ok) throw new Error(data.error || "request failed");
  return data;
}

function escapeHtml(text) {
  return String(text || "").replace(/[&<>"']/g, ch => ({
    "&": "&amp;",
    "<": "&lt;",
    ">": "&gt;",
    "\"": "&quot;",
    "'": "&#039;",
  }[ch]));
}

function showToast(text, kind = "ok") {
  clearTimeout(state.toastTimer);
  els.toast.textContent = text;
  els.toast.className = `toast ${kind}`;
  state.toastTimer = setTimeout(() => els.toast.classList.add("hidden"), 2600);
}

function setBusy(button, busyText) {
  const oldText = button.textContent;
  button.disabled = true;
  button.textContent = busyText;
  return () => {
    button.disabled = false;
    button.textContent = oldText;
  };
}

function acceptedFriends() {
  return state.friends.filter(friend => friend.status === "accepted");
}

function knownFriend(username) {
  return state.friends.find(friend => friend.username.toLowerCase() === String(username).toLowerCase());
}

function chatFor(username) {
  return state.chats.find(chat => chat.peer.toLowerCase() === String(username).toLowerCase());
}

function displayName(user) {
  const realName = [user.first_name, user.last_name].filter(Boolean).join(" ");
  return realName || user.display_name || user.username;
}

function profileLine(user) {
  return [user.position, user.location, user.bio].filter(Boolean).join(" | ") || user.role || "user";
}

async function refresh() {
  const data = await api("/api/me");
  state.csrf = data.csrf;
  state.user = data.user;
  state.chats = data.chats || [];
  state.friends = data.friends || [];

  if (!state.user) {
    els.auth.classList.remove("hidden");
    els.app.classList.add("hidden");
    return;
  }

  if (location.pathname === "/") {
    location.replace("/chats");
    return;
  }

  els.auth.classList.add("hidden");
  els.app.classList.remove("hidden");
  els.me.textContent = `@${state.user.username}`;
  els.role.textContent = state.user.role;
  els.avatar.textContent = state.user.username.slice(0, 1).toUpperCase();
  renderPage();
}

function renderPage() {
  const current = route();
  for (const [name, page] of Object.entries(els.pages)) {
    page.classList.toggle("hidden", name !== current.page);
  }
  for (const link of els.navLinks) {
    link.classList.toggle("active", link.dataset.page === current.page);
  }

  if (current.page === "chats") renderChatsPage();
  if (current.page === "friends") renderFriendsPage();
  if (current.page === "profile") renderProfilePage();
  if (current.page === "chat") renderChatPage(current.username);
}

function renderChatsPage() {
  els.chatCount.textContent = `${state.chats.length} chats`;
  renderChatList();
  renderChatSearch();
  renderSupportInbox(null);
  if (state.user.role === "support") {
    els.supportInbox.classList.remove("hidden");
  } else {
    els.supportInbox.classList.add("hidden");
  }
}

function renderChatList() {
  els.chatList.innerHTML = "";
  if (!state.chats.length) {
    els.chatList.append(emptyBox("No chats yet"));
    return;
  }

  for (const chat of state.chats) {
    const row = document.createElement("a");
    row.className = "row";
    row.href = `/chat/${encodeURIComponent(chat.peer)}`;
    row.innerHTML = `
      <span class="row-main">
        <strong>@${escapeHtml(chat.peer)}</strong>
        <small>${escapeHtml(chat.last_message || "No messages yet")}</small>
      </span>
      ${chat.unread ? `<span class="badge">${chat.unread}</span>` : "<span></span>"}
    `;
    els.chatList.append(row);
  }
}

function renderChatSearch() {
  const q = els.chatSearch.value.trim().toLowerCase();
  els.chatSearchPanel.innerHTML = "";
  els.chatSearchPanel.classList.toggle("hidden", !q);
  els.chatList.classList.toggle("hidden", Boolean(q));
  if (!q) return;

  const matches = acceptedFriends()
    .filter(friend => friend.username.toLowerCase().includes(q) || displayName(friend).toLowerCase().includes(q))
    .slice(0, 10);

  if (!matches.length) {
    els.chatSearchPanel.append(emptyBox("No friends found"));
    return;
  }

  for (const friend of matches) {
    els.chatSearchPanel.append(personRow(friend, "Open", `/chat/${encodeURIComponent(friend.username)}`));
  }
}

function renderSupportInbox(ticket) {
  els.ticketList.innerHTML = "";
  els.ticketCount.textContent = "lookup";
  if (!ticket) {
    els.ticketList.append(emptyBox("Enter a ticket uid to load a report"));
    return;
  }

  const row = document.createElement("div");
  row.className = "row";
  row.innerHTML = `
    <span class="row-main">
      <strong>${escapeHtml(ticket.ticket_uid)} from @${escapeHtml(ticket.customer)}</strong>
      <small>${escapeHtml(ticket.subject)} · ${escapeHtml(ticket.last_note || ticket.status)}</small>
    </span>
    <span class="row-actions"></span>
  `;
  const customerLink = document.createElement("a");
  customerLink.className = "link-button secondary";
  customerLink.href = `/chat/${encodeURIComponent(ticket.customer)}`;
  customerLink.textContent = "Customer";
  const watch = document.createElement("button");
  watch.className = "secondary";
  watch.textContent = "Watch";
  watch.addEventListener("click", () => {
    sendWs({
      op: "watch_ticket_thread",
      ticket_uid: ticket.ticket_uid,
      thread: ticket.review_thread,
    }).catch(err => showToast(err.message, "error"));
  });
  row.querySelector(".row-actions").append(customerLink, watch);
  els.ticketList.append(row);
}

function renderFriendsPage() {
  els.friendCount.textContent = `${acceptedFriends().length} friends`;
  renderFriendList();
  renderFriendSearch();
}

function renderFriendList() {
  els.friendList.innerHTML = "";
  const friends = acceptedFriends();
  if (!friends.length) {
    els.friendList.append(emptyBox("No friends yet"));
    return;
  }

  for (const friend of friends) {
    els.friendList.append(personRow(friend, "Chat", `/chat/${encodeURIComponent(friend.username)}`));
  }
}

async function renderFriendSearch() {
  const q = els.friendSearch.value.trim();
  els.friendSearchPanel.innerHTML = "";
  els.friendSearchPanel.classList.toggle("hidden", !q);
  els.friendList.classList.toggle("hidden", Boolean(q));
  if (!q) return;

  try {
    const data = await api(`/api/users/search?q=${encodeURIComponent(q)}`);
    if (els.friendSearch.value.trim() !== q) return;
    const users = data.users.filter(user => !knownFriend(user.username));
    if (!users.length) {
      els.friendSearchPanel.append(emptyBox("No new people found"));
      return;
    }

    for (const user of users) {
      const row = document.createElement("div");
      row.className = "row";
      row.innerHTML = `
        <span class="row-main">
          <strong>@${escapeHtml(user.username)}</strong>
          <small>${escapeHtml(profileLine(user))}</small>
        </span>
        <span class="row-actions"></span>
      `;
      const action = document.createElement("button");
      action.textContent = "Add";
      action.addEventListener("click", async () => {
        const done = setBusy(action, "Adding...");
        try {
          await api("/api/friends/request", {
            method: "POST",
            body: JSON.stringify({ username: user.username }),
          });
          showToast("Friend added");
          els.friendSearch.value = "";
          await refresh();
        } catch (err) {
          showToast(err.message, "error");
        } finally {
          done();
        }
      });
      row.querySelector(".row-actions").append(action);
      els.friendSearchPanel.append(row);
    }
  } catch (err) {
    showToast(err.message, "error");
  }
}

function renderProfilePage() {
  els.role.textContent = state.user.role;
  els.currentStatus.textContent = state.user.bio || "No status yet";
  els.firstName.value = state.user.first_name || "";
  els.lastName.value = state.user.last_name || "";
  els.position.value = state.user.position || "";
  els.locationField.value = state.user.location || "";
  els.statusInput.value = "";
}

function renderChatPage(username) {
  els.chatTitle.textContent = `@${username}`;
  els.chatSubtitle.textContent = "Loading conversation";
  els.messages.innerHTML = "";
  els.messages.append(emptyBox("Loading messages"));
  els.messageInput.disabled = true;
  els.sendButton.disabled = true;
  els.report.classList.toggle("hidden", username !== "support");
  openDm(username);
}

function personRow(user, actionText, href) {
  const row = document.createElement("div");
  row.className = "row";
  row.innerHTML = `
    <span class="row-main">
      <strong>@${escapeHtml(user.username)}</strong>
      <small>${escapeHtml(profileLine(user))}</small>
    </span>
    <span class="row-actions"></span>
  `;
  const link = document.createElement("a");
  link.className = "link-button secondary";
  link.href = href;
  link.textContent = actionText;
  row.querySelector(".row-actions").append(link);
  return row;
}

function emptyBox(text) {
  const box = document.createElement("div");
  box.className = "empty-inline";
  box.textContent = text;
  return box;
}

function connectWs() {
  if (state.ws && state.ws.readyState <= WebSocket.OPEN) return state.ws;
  const proto = location.protocol === "https:" ? "wss:" : "ws:";
  state.ws = new WebSocket(`${proto}//${location.host}/ws`);
  state.ws.addEventListener("message", event => {
    const msg = JSON.parse(event.data);
    if (msg.type === "dm_opened") {
      state.activeChat = { id: msg.chat_id, peer: msg.peer.username };
      renderConversation(msg);
    }
    if (msg.type === "message") {
      if (state.activeChat && msg.message.chat_id === state.activeChat.id) {
        appendMessage(msg.message);
        sendWs({ op: "read_chat", chat_id: state.activeChat.id }).catch(() => {});
      }
    }
    if (msg.type === "inbox_sync") {
      renderSupportInbox(msg.ticket);
    }
    if (msg.type === "thread_event") {
      showToast(`Review thread attached to @${msg.peer}`);
    }
    if (msg.type === "error") {
      els.messages.innerHTML = "";
      els.messages.append(emptyBox("Conversation is unavailable"));
      showToast(msg.error, "error");
    }
  });
  state.ws.addEventListener("close", () => {
    state.ws = null;
  });
  return state.ws;
}

function waitForWs() {
  const ws = connectWs();
  if (ws.readyState === WebSocket.OPEN) return Promise.resolve(ws);
  return new Promise((resolve, reject) => {
    const timer = setTimeout(() => reject(new Error("connection timeout")), 5000);
    ws.addEventListener("open", () => {
      clearTimeout(timer);
      resolve(ws);
    }, { once: true });
    ws.addEventListener("error", () => {
      clearTimeout(timer);
      reject(new Error("connection failed"));
    }, { once: true });
  });
}

async function sendWs(payload) {
  const ws = await waitForWs();
  ws.send(JSON.stringify(payload));
}

async function openDm(username) {
  try {
    await sendWs({ op: "open_dm", username });
  } catch (err) {
    showToast(err.message, "error");
  }
}

function renderConversation(data) {
  els.chatTitle.textContent = `@${data.peer.username}`;
  els.chatSubtitle.textContent = displayName(data.peer);
  els.messages.innerHTML = "";
  if (!data.messages.length) {
    els.messages.append(emptyBox("No messages yet"));
  } else {
    for (const message of data.messages) appendMessage(message);
  }
  els.messageInput.disabled = false;
  els.sendButton.disabled = false;
  els.report.classList.toggle("hidden", data.peer.username !== "support");
  els.messages.scrollTop = els.messages.scrollHeight;
}

function appendMessage(message) {
  const oldEmpty = els.messages.querySelector(".empty-inline");
  if (oldEmpty) oldEmpty.remove();
  const box = document.createElement("div");
  box.className = `message ${message.sender === state.user.username ? "own" : ""}`;
  box.innerHTML = `<small>@${escapeHtml(message.sender)}</small>${escapeHtml(message.body)}`;
  els.messages.append(box);
  els.messages.scrollTop = els.messages.scrollHeight;
}

async function submitAuth(form, endpoint) {
  els.authError.textContent = "";
  const button = form.querySelector("button");
  const done = setBusy(button, endpoint.includes("login") ? "Signing in..." : "Creating...");
  const formData = new FormData(form);
  try {
    const data = await api(endpoint, {
      method: "POST",
      body: JSON.stringify(Object.fromEntries(formData.entries())),
    });
    state.csrf = data.csrf;
    location.href = location.pathname === "/" ? "/chats" : location.pathname;
  } catch (err) {
    els.authError.textContent = err.message;
  } finally {
    done();
  }
}

els.login.addEventListener("submit", event => {
  event.preventDefault();
  submitAuth(els.login, "/api/login");
});

els.register.addEventListener("submit", event => {
  event.preventDefault();
  submitAuth(els.register, "/api/register");
});

els.logout.addEventListener("click", async () => {
  const done = setBusy(els.logout, "Signing out...");
  try {
    await api("/api/logout", { method: "POST" });
    location.href = "/";
  } catch (err) {
    showToast(err.message, "error");
    done();
  }
});

els.chatSearch.addEventListener("input", renderChatSearch);
els.friendSearch.addEventListener("input", () => {
  clearTimeout(els.friendSearch.timer);
  if (!els.friendSearch.value.trim()) {
    renderFriendSearch();
    return;
  }
  els.friendSearch.timer = setTimeout(renderFriendSearch, 180);
});

els.profileForm.addEventListener("submit", async event => {
  event.preventDefault();
  const done = setBusy(els.saveProfile, "Saving...");
  const status = els.statusInput.value.trim();
  const payload = {
    first_name: els.firstName.value,
    last_name: els.lastName.value,
    position: els.position.value,
    location: els.locationField.value,
  };
  if (status) payload.bio = status;

  try {
    const data = await api("/api/profile", {
      method: "POST",
      body: JSON.stringify(payload),
    });
    state.user = data.user;
    renderProfilePage();
    showToast("Profile saved");
  } catch (err) {
    showToast(err.message, "error");
  } finally {
    done();
  }
});

els.messageForm.addEventListener("submit", async event => {
  event.preventDefault();
  const body = els.messageInput.value.trim();
  if (!body || !state.activeChat) return;
  const done = setBusy(els.sendButton, "Sending...");
  try {
    await sendWs({
      op: "send_message",
      chat_id: state.activeChat.id,
      body,
    });
    els.messageInput.value = "";
  } catch (err) {
    showToast(err.message, "error");
  } finally {
    done();
  }
});

els.ticketUidInput.addEventListener("input", () => {
  clearTimeout(els.ticketUidInput.timer);
  const ticketUid = els.ticketUidInput.value.trim();
  if (!ticketUid) {
    renderSupportInbox(null);
    return;
  }
  els.ticketUidInput.timer = setTimeout(() => {
    sendWs({ op: "sync_inbox", ticket_uid: ticketUid }).catch(err => showToast(err.message, "error"));
  }, 180);
});

els.report.addEventListener("click", async () => {
  if (!state.activeChat) return;
  const body = els.messageInput.value.trim() || "Please review this conversation.";
  const done = setBusy(els.report, "Reporting...");
  try {
    await sendWs({
      op: "send_message",
      chat_id: state.activeChat.id,
      body: `/report ${body}`,
    });
    els.messageInput.value = "";
    showToast("Report sent");
  } catch (err) {
    showToast(err.message, "error");
  } finally {
    done();
  }
});

refresh().catch(() => {});
