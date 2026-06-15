<?php
declare(strict_types=1);

require_once __DIR__ . '/../includes/db.php';
require_once __DIR__ . '/../includes/auth.php';
require_once __DIR__ . '/../includes/webauthn.php';
require_once __DIR__ . '/../includes/betting.php';

// Friendly "starting up" page while DB initialises
if (!isset($_GET['action']) && !isset($_POST['action'])) {
    try { db(); } catch (\Throwable $e) {
        http_response_code(503);
        header('Retry-After: 5');
        echo '<!DOCTYPE html><html lang="ru"><head><meta charset="UTF-8">
        <meta http-equiv="refresh" content="5"><title>1xVKA — Запуск…</title>
        <style>body{background:#0d1117;color:#e6edf3;font-family:sans-serif;display:flex;align-items:center;justify-content:center;height:100vh;flex-direction:column;gap:16px}
        .spin{font-size:40px;animation:s 1s linear infinite}@keyframes s{to{transform:rotate(360deg)}}</style></head>
        <body><div class="spin">⚙️</div><h1>1xVKA запускается…</h1>
        <p style="color:#8b949e">База данных инициализируется. Страница обновится автоматически.</p></body></html>';
        exit;
    }
}

session_init();

$page   = $_GET['page']   ?? 'home';
$action = $_POST['action'] ?? $_GET['action'] ?? '';

// ─── JSON API ──────────────────────────────────────────────────────────────
if ($action) {
    while (ob_get_level()) ob_end_clean();
    ob_start();
    header('Content-Type: application/json; charset=utf-8');

    set_exception_handler(function(\Throwable $e) {
        while (ob_get_level()) ob_end_clean();
        http_response_code(500);
        header('Content-Type: application/json; charset=utf-8');
        echo json_encode(['ok'=>false,'error'=>$e->getMessage()]);
        exit;
    });

    try {
        $out = match($action) {

            // ── User auth ──────────────────────────────────────────────────
            'register' => (function() {
                if (!csrf_ok($_POST['csrf']??'')) return ['ok'=>false,'error'=>'CSRF'];
                $r = register_user(trim($_POST['username']??''), trim($_POST['email']??''), $_POST['password']??'');
                if ($r['ok']) { session_regenerate_id(true); $_SESSION['uid'] = $r['user_id']; }
                return $r;
            })(),

            'login' => (function() {
                if (!csrf_ok($_POST['csrf']??'')) return ['ok'=>false,'error'=>'CSRF'];
                return login_user(trim($_POST['login']??''), $_POST['password']??'');
            })(),

            'logout' => (function() { logout_user(); return ['ok'=>true]; })(),

            // ── User WebAuthn ──────────────────────────────────────────────
            'wa_reg_options' => (function() {
                $u = current_user();
                if (!$u) return ['ok'=>false,'error'=>'Необходима авторизация'];
                return webauthn_registration_options($u);
            })(),

            'wa_reg_verify' => (function() {
                $u = current_user();
                if (!$u) return ['ok'=>false,'error'=>'Необходима авторизация'];
                $d = json_decode(file_get_contents('php://input')?:'{}', true) ?? [];
                return webauthn_verify_registration($u, $d);
            })(),

            'wa_auth_options' => (function() {
                $uid = (int)($_SESSION['wa_pending'] ?? 0);
                if (!$uid) return ['ok'=>false,'error'=>'Нет ожидающего входа'];
                return webauthn_auth_options($uid);
            })(),

            'wa_auth_verify' => (function() {
                $d   = json_decode(file_get_contents('php://input')?:'{}', true) ?? [];
                $res = webauthn_verify_auth($d);
                if ($res['ok']) complete_webauthn_login($res['user_id']);
                return $res;
            })(),

            // ── Bets ──────────────────────────────────────────────────────
            'place_bet' => (function() {
                $u = current_user();
                if (!$u) return ['ok'=>false,'error'=>'Необходима авторизация'];
                if (!csrf_ok($_POST['csrf']??'')) return ['ok'=>false,'error'=>'CSRF'];
                return place_bet($u['id'], (int)($_POST['match_id']??0), (int)($_POST['athlete_id']??0), (float)($_POST['amount']??0));
            })(),

            'get_matches' => (function() {
                generate_new_matches();
                return get_open_matches();
            })(),

            'get_bets' => (function() {
                $u = current_user();
                if (!$u) return ['ok'=>false,'error'=>'Необходима авторизация'];
                return get_bets_for_user($u['id']);
            })(),

            'get_balance' => (function() {
                $u = current_user();
                if (!$u) return ['ok'=>false,'error'=>'Необходима авторизация'];
                return ['balance' => number_format((float)$u['balance'], 2, '.', '')];
            })(),

            // ── Admin: step 1 — password ───────────────────────────────────
            'admin_login' => (function() {
                if (!csrf_ok($_POST['csrf']??'')) return ['ok'=>false,'error'=>'CSRF'];
                return admin_password_check(trim($_POST['username']??''), $_POST['password']??'');
            })(),

            // ── Admin: step 2a — WebAuthn challenge ────────────────────────
            'admin_wa_options' => (function() {
                if (!admin_pw_verified()) return ['ok'=>false,'error'=>'Сначала введите пароль'];
                return admin_wa_auth_options();
            })(),

            // ── Admin: step 2b — WebAuthn verify ──────────────────────────
            'admin_wa_verify' => (function() {
                if (!admin_pw_verified()) return ['ok'=>false,'error'=>'Сначала введите пароль'];
                $d   = json_decode(file_get_contents('php://input')?:'{}', true) ?? [];
                $res = admin_wa_verify($d);
                if ($res['ok']) admin_complete_login();
                return $res;
            })(),

            // ── Admin: logout ──────────────────────────────────────────────
            'admin_logout' => (function() { admin_logout(); return ['ok'=>true]; })(),

            // ── Admin: user management ─────────────────────────────────────
            'admin_adjust_balance' => (function() {
                if (!is_admin()) return ['ok'=>false,'error'=>'Нет доступа'];
                if (!csrf_ok($_POST['csrf']??'')) return ['ok'=>false,'error'=>'CSRF'];
                $uid = (int)($_POST['user_id']??0);
                $amt = (float)($_POST['amount']??0);
                if (!$uid || $amt == 0.0) return ['ok'=>false,'error'=>'Неверные параметры'];
                $db = db();
                $st = $db->prepare('SELECT balance FROM users WHERE id=?'); $st->execute([$uid]);
                if ((float)($st->fetchColumn() ?: 0) + $amt < 0)
                    return ['ok'=>false,'error'=>'Баланс не может быть отрицательным'];
                $db->prepare('UPDATE users SET balance=balance+? WHERE id=?')->execute([$amt,$uid]);
                return ['ok'=>true];
            })(),

            default => ['ok'=>false,'error'=>'Unknown action'],
        };
    } catch(\Throwable $e) {
        $out = ['ok'=>false,'error'=>$e->getMessage()];
    }

    while (ob_get_level() > 1) ob_end_clean();
    ob_end_clean();
    echo json_encode($out);
    exit;
}

// ─── Page rendering ────────────────────────────────────────────────────────
$user  = current_user();
$admin = is_admin();
$csrf  = csrf_token();
?>
<!DOCTYPE html>
<html lang="ru">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>1xVKA — летние ставки</title>
<link rel="stylesheet" href="/css/style.css">
</head>
<body>
<nav class="navbar">
  <div class="nav-brand">🏅 <span class="brand-name">1xVKA</span></div>
  <div class="nav-links">
    <a href="/" class="nav-link <?= $page==='home'?'active':'' ?>">🏠 Матчи</a>
    <?php if ($user): ?>
    <a href="/?page=bets"    class="nav-link <?= $page==='bets'   ?'active':'' ?>">📋 Ставки</a>
    <a href="/?page=profile" class="nav-link <?= $page==='profile'?'active':'' ?>">⚙️ Профиль</a>
    <span class="nav-balance">💰 <span id="bal"><?= number_format((float)$user['balance'],2,'.',' ') ?></span></span>
    <button onclick="doLogout()" class="btn btn-sm">Выйти</button>
    <?php else: ?>
    <a href="/?page=login"    class="btn btn-sm">Войти</a>
    <a href="/?page=register" class="btn btn-primary btn-sm">Регистрация</a>
    <?php endif; ?>
    <?php if ($admin): ?>
    <a href="/?page=admin" class="nav-link admin-link">🛡 Админ</a>
    <?php endif; ?>
  </div>
</nav>
<main class="wrap">
<?php
switch ($page) {
    case 'register':
        if ($user) { header('Location:/'); exit; }
        include __DIR__ . '/../templates/register.php'; break;
    case 'login':
        if ($user) { header('Location:/'); exit; }
        include __DIR__ . '/../templates/login.php'; break;
    case 'bets':
        if (!$user) { header('Location:/?page=login'); exit; }
        include __DIR__ . '/../templates/bets.php'; break;
    case 'profile':
        if (!$user) { header('Location:/?page=login'); exit; }
        include __DIR__ . '/../templates/profile.php'; break;
    case 'admin_login':
        if ($admin) { header('Location:/?page=admin'); exit; }
        include __DIR__ . '/../templates/admin_login.php'; break;
    case 'admin':
        if (!$admin) { header('Location:/?page=admin_login'); exit; }
        include __DIR__ . '/../templates/admin.php'; break;
    default:
        include __DIR__ . '/../templates/home.php';
}
?>
</main>
<footer class="footer">
  <span>🏅 1xVKA — виртуальные ставки · летние олимпийские игры</span>
  <a href="/?page=admin_login" class="admin-footer-link">🛡 Вход для администратора</a>
</footer>
<div id="toast" class="toast hidden"></div>
<script src="/js/app.js"></script>
</body>
</html>
