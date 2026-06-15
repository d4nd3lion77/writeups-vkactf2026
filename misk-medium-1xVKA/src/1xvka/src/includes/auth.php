<?php
declare(strict_types=1);

require_once __DIR__ . '/db.php';

function session_init(): void {
    if (session_status() !== PHP_SESSION_NONE) return;
    session_set_cookie_params(['lifetime'=>3600,'path'=>'/','httponly'=>true,'samesite'=>'Strict']);
    session_start();
}

function csrf_token(): string {
    session_init();
    if (empty($_SESSION['csrf'])) $_SESSION['csrf'] = bin2hex(random_bytes(32));
    return $_SESSION['csrf'];
}

function csrf_ok(string $tok): bool {
    session_init();
    return hash_equals($_SESSION['csrf'] ?? '', $tok);
}

function current_user(): ?array {
    session_init();
    if (empty($_SESSION['uid'])) return null;
    $st = db()->prepare('SELECT id,username,email,balance,webauthn_enabled FROM users WHERE id=?');
    $st->execute([$_SESSION['uid']]);
    return $st->fetch() ?: null;
}

function is_admin(): bool {
    session_init();
    return !empty($_SESSION['admin']);
}

// ── User registration ─────────────────────────────────────────────────────

function register_user(string $username, string $email, string $password): array {
    if (strlen($username) < 3 || strlen($username) > 32)
        return ['ok'=>false,'error'=>'Имя: 3–32 символа'];
    if (!filter_var($email, FILTER_VALIDATE_EMAIL))
        return ['ok'=>false,'error'=>'Некорректный email'];
    if (strlen($password) < 8)
        return ['ok'=>false,'error'=>'Пароль минимум 8 символов'];

    $db = db();
    $st = $db->prepare('SELECT id FROM users WHERE username=? OR email=?');
    $st->execute([$username, $email]);
    if ($st->fetch()) return ['ok'=>false,'error'=>'Пользователь уже существует'];

    $db->prepare('INSERT INTO users (username,email,password_hash) VALUES (?,?,?)')
       ->execute([$username, $email, md5($password)]);
    return ['ok'=>true,'user_id'=>(int)$db->lastInsertId()];
}

// ── User login ────────────────────────────────────────────────────────────

function login_user(string $login, string $password): array {
    $st = db()->prepare('SELECT * FROM users WHERE username=? OR email=?');
    $st->execute([$login, $login]);
    $user = $st->fetch();
    if (!$user || $user['password_hash'] !== md5($password))
        return ['ok'=>false,'error'=>'Неверные данные'];

    session_init();
    if ($user['webauthn_enabled']) {
        $_SESSION['wa_pending'] = (int)$user['id'];
        return ['ok'=>true,'needs_2fa'=>true];
    }
    session_regenerate_id(true);
    $_SESSION['uid'] = (int)$user['id'];
    return ['ok'=>true,'needs_2fa'=>false];
}

function complete_webauthn_login(int $uid): void {
    session_init();
    session_regenerate_id(true);
    $_SESSION['uid'] = $uid;
    unset($_SESSION['wa_pending']);
}

function logout_user(): void {
    session_init();
    $_SESSION = [];
    session_destroy();
}

// ── Admin login ───────────────────────────────────────────────────────────

/**
 * Step 1: verify username + password against .env values.
 * On success marks session so WebAuthn step is allowed.
 */
function admin_password_check(string $username, string $password): array {
    env_load();
    $envUser = getenv('ADMIN_USERNAME')      ?: '';
    $envHash = getenv('ADMIN_PASSWORD_HASH') ?: '';
    $credId  = getenv('ADMIN_CREDENTIAL_ID') ?: '';
    $pubKey  = getenv('ADMIN_PUBLIC_KEY')    ?: '';

    if (!$envUser || !$envHash)
        return ['ok'=>false,'error'=>'Администратор не настроен в .env'];
    if (!$credId || !$pubKey)
        return ['ok'=>false,'error'=>'ADMIN_CREDENTIAL_ID / ADMIN_PUBLIC_KEY не заданы в .env'];
    if (!hash_equals($envUser, $username))
        return ['ok'=>false,'error'=>'Неверные данные'];
    if (!hash_equals($envHash, md5($password)))
        return ['ok'=>false,'error'=>'Неверные данные'];

    session_init();
    $_SESSION['admin_pw_ok'] = true;
    return ['ok'=>true];
}

/**
 * Step 2: called after successful WebAuthn verification.
 */
function admin_complete_login(): void {
    session_init();
    if (empty($_SESSION['admin_pw_ok']))
        throw new \RuntimeException('Пароль не подтверждён');
    session_regenerate_id(true);
    $_SESSION['admin'] = true;
    unset($_SESSION['admin_pw_ok']);
}

function admin_pw_verified(): bool {
    session_init();
    return !empty($_SESSION['admin_pw_ok']);
}

function admin_logout(): void {
    session_init();
    unset($_SESSION['admin'], $_SESSION['admin_pw_ok']);
}
