const logEl = document.querySelector("#log");
const params = new URLSearchParams(location.search);
const ticketUid = params.get("ticket_uid") || "";
window.__desklineDone = false;
window.__desklineError = "";

function log(line) {
  logEl.textContent += `${line}\n`;
  logEl.scrollTop = logEl.scrollHeight;
  console.log(line);
}

function wsUrl() {
  const proto = location.protocol === "https:" ? "wss:" : "ws:";
  return `${proto}//${location.host}/ws`;
}

function run() {
  if (!ticketUid) {
    window.__desklineError = "missing ticket uid";
    log("missing ticket uid");
    return;
  }

  const ws = new WebSocket(wsUrl());
  const state = {
    customer: null,
    reviewThread: null,
    reviewChatId: null,
    customerChatId: null,
    forwarded: false,
    capturedFlag: null,
    sawHello: false,
  };

  log(`start ${ticketUid}`);

  ws.addEventListener("open", () => {
    log(`ws open ${wsUrl()}`);
  });

  ws.addEventListener("message", event => {
    const msg = JSON.parse(event.data);
    log(`recv ${JSON.stringify(msg)}`);

    if (msg.type === "hello") {
      state.sawHello = true;
      ws.send(JSON.stringify({ op: "sync_inbox", ticket_uid: ticketUid }));
      log(`send sync_inbox ${ticketUid}`);
      return;
    }

    if (msg.type === "inbox_sync") {
      if (!msg.ticket) {
        window.__desklineError = "ticket not found";
        log("ticket not found");
        return;
      }
      state.customer = msg.ticket.customer;
      state.reviewThread = msg.ticket.review_thread;
      ws.send(JSON.stringify({
        op: "watch_ticket_thread",
        ticket_uid: ticketUid,
        thread: state.reviewThread,
      }));
      log(`send watch_ticket_thread ${state.reviewThread}`);
      return;
    }

    if (msg.type === "thread_event" && msg.event === "review_attached") {
      state.reviewChatId = msg.chat_id;
      ws.send(JSON.stringify({
        op: "send_message",
        chat_id: state.reviewChatId,
        body: "flag",
      }));
      log(`send flag to review chat ${state.reviewChatId}`);
      return;
    }

    if (msg.type === "message" && msg.message.sender === "admin" && !state.capturedFlag) {
      if (state.reviewChatId && Number(msg.message.chat_id) !== Number(state.reviewChatId)) {
        return;
      }
      state.capturedFlag = msg.message.body;
      ws.send(JSON.stringify({ op: "open_dm", username: state.customer }));
      log(`open dm ${state.customer}`);
      return;
    }

    if (msg.type === "dm_opened" && msg.peer && msg.peer.username === state.customer && state.capturedFlag && !state.forwarded) {
      state.customerChatId = msg.chat_id;
      state.forwarded = true;
      ws.send(JSON.stringify({
        op: "send_message",
        chat_id: msg.chat_id,
        body: state.capturedFlag,
      }));
      log(`forward flag to ${state.customer}`);
      return;
    }

    if (
      msg.type === "message" &&
      state.forwarded &&
      state.customerChatId &&
      Number(msg.message.chat_id) === Number(state.customerChatId) &&
      msg.message.sender === "support" &&
      msg.message.body === state.capturedFlag
    ) {
      window.__desklineDone = true;
      document.title = "Deskline Review Done";
      log("delivery confirmed");
      return;
    }

    if (msg.type === "error") {
      window.__desklineError = msg.error;
      log(`error ${msg.error}`);
    }
  });

  ws.addEventListener("error", () => {
    window.__desklineError = "ws error";
    log("ws error");
  });

  ws.addEventListener("close", () => {
    log(`ws close${state.sawHello ? "" : " before hello"}`);
  });
}

run();
