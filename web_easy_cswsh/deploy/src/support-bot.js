const puppeteer = require("puppeteer-core");
const config = require("./config");

const queue = [];
let activeWorkers = 0;
let visitSeq = 0;
const baseOrigin = new URL(config.BASE_URL).origin;

function allowedUrl(value) {
  try {
    const url = new URL(value);
    if (url.username || url.password) return false;
    if (url.protocol !== "http:" && url.protocol !== "https:") return false;
    return url.origin !== baseOrigin;
  } catch {
    return false;
  }
}

function enqueueVisit(url) {
  if (!allowedUrl(url)) return false;
  queue.push({ id: ++visitSeq, url });
  pumpQueue();
  return true;
}

function launchBrowser() {
  return puppeteer.launch({
    executablePath: config.CHROME_PATH,
    headless: "new",
    args: [
      "--no-sandbox",
      "--disable-dev-shm-usage",
      "--disable-gpu",
      "--disable-setuid-sandbox",
      "--no-first-run",
      "--no-default-browser-check",
      "--disable-features=BlockThirdPartyCookies,ThirdPartyStoragePartitioning,TrackingProtection3pcd,TrackingProtection3pcdUx",
    ],
  });
}

async function loginSupport(page, visitId) {
  await page.goto(config.BASE_URL, { waitUntil: "domcontentloaded", timeout: 15000 });
  const login = await page.evaluate(async ({ username, password }) => {
    const meta = await fetch("/api/me").then(r => r.json());
    const res = await fetch("/api/login", {
      method: "POST",
      headers: {
        "content-type": "application/json",
        "x-csrf-token": meta.csrf,
      },
      body: JSON.stringify({ username, password }),
    });
    let body = "";
    try {
      body = await res.text();
    } catch {
      body = "";
    }
    return {
      ok: res.ok,
      status: res.status,
      body,
      csrf: meta.csrf || "",
    };
  }, { username: "support", password: config.SUPPORT_PASSWORD });
  if (!login.ok) {
    throw new Error(`support login failed (${login.status}): ${login.body || "empty response"}`);
  }
  const cookies = await page.cookies(config.BASE_URL);
  console.log("support login ok", {
    visitId,
    baseUrl: config.BASE_URL,
    hasSessionCookie: cookies.some(cookie => cookie.name === "deskline.sid"),
  });
}

async function visit(job) {
  const browser = await launchBrowser();
  let page = null;
  let timeoutId = null;

  try {
    page = await browser.newPage();
    await page.setCacheEnabled(false);
    page.on("console", message => {
      console.log(`support page console [${job.id}]:`, message.type(), message.text());
    });
    page.on("pageerror", err => {
      console.error(`support page error [${job.id}]:`, err.message);
    });
    page.on("requestfailed", request => {
      console.error(
        `support request failed [${job.id}]:`,
        request.url(),
        request.failure()?.errorText || "unknown",
      );
    });
    page.setDefaultTimeout(12000);

    const visitFlow = (async () => {
      await loginSupport(page, job.id);
      await page.goto(job.url, { waitUntil: "domcontentloaded", timeout: 12000 });
      await page.waitForFunction(() => window.__desklineDone === true, {
        timeout: config.SUPPORT_VISIT_TIMEOUT_MS,
      });
      console.log("support visit done", { visitId: job.id, url: job.url });
    })();

    const timeoutFlow = new Promise((_, reject) => {
      timeoutId = setTimeout(() => {
        reject(new Error(`support visit timeout after ${config.SUPPORT_VISIT_TIMEOUT_MS}ms`));
      }, config.SUPPORT_VISIT_TIMEOUT_MS + 3000);
    });

    await Promise.race([visitFlow, timeoutFlow]);
  } catch (err) {
    let state = null;
    if (page) {
      state = await page.evaluate(() => ({
        href: location.href,
        done: Boolean(window.__desklineDone),
        error: window.__desklineError || "",
        title: document.title,
      })).catch(() => null);
    }
    console.error("support visit failed:", {
      visitId: job.id,
      url: job.url,
      error: err.message,
      state,
    });
  } finally {
    if (timeoutId) clearTimeout(timeoutId);
    if (page) {
      await page.close().catch(() => {});
    }
    await browser.close().catch(() => {});
  }
}

function pumpQueue() {
  while (activeWorkers < config.SUPPORT_WORKERS && queue.length) {
    const job = queue.shift();
    activeWorkers += 1;
    console.log("support visit queued", {
      visitId: job.id,
      url: job.url,
      activeWorkers,
      queueLeft: queue.length,
    });

    visit(job)
      .catch(err => {
        console.error("support worker crash", {
          visitId: job.id,
          error: err.message,
        });
      })
      .finally(() => {
        activeWorkers -= 1;
        pumpQueue();
      });
  }
}

if (require.main === module) {
  const url = process.argv[2];
  if (!url) {
    console.error("usage: npm run bot -- <url>");
    process.exit(1);
  }
  enqueueVisit(url);
}

module.exports = { enqueueVisit, canVisitUrl: allowedUrl };
