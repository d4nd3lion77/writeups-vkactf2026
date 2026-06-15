const path = require("path");

const PORT = Number(process.env.PORT || 3000);
const BASE_URL = process.env.BASE_URL || `http://127.0.0.1:${PORT}`;
const DATA_DIR = process.env.DATA_DIR || path.join(__dirname, "..", "data");
const cookieSameSite = process.env.COOKIE_SAMESITE === "false"
  ? false
  : (process.env.COOKIE_SAMESITE || "lax");

module.exports = {
  PORT,
  BASE_URL,
  DATA_DIR,
  DB_PATH: process.env.DB_PATH || path.join(DATA_DIR, "deskline.db"),
  FLAG_PATH: process.env.FLAG_PATH || path.join(__dirname, "..", "flag.txt"),
  SESSION_SECRET: process.env.SESSION_SECRET || "dev-secret",
  ADMIN_PASSWORD: process.env.ADMIN_PASSWORD || "admin-password",
  SUPPORT_PASSWORD: process.env.SUPPORT_PASSWORD || "support-password",
  CHROME_PATH: process.env.CHROME_PATH || "/usr/bin/chromium",
  SUPPORT_WORKERS: Math.max(1, Number(process.env.SUPPORT_WORKERS || 3)),
  SUPPORT_VISIT_TIMEOUT_MS: Math.max(5000, Number(process.env.SUPPORT_VISIT_TIMEOUT_MS || 20000)),
  COOKIE_SECURE: process.env.COOKIE_SECURE === "true",
  COOKIE_SAMESITE: cookieSameSite,
};
