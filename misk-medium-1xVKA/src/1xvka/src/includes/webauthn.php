<?php
declare(strict_types=1);

require_once __DIR__ . '/db.php';
require_once __DIR__ . '/auth.php';

/**
 * WebAuthn implementation.
 * Uses web-auth/webauthn-framework CBOR/COSE libs + OpenSSL fallback.
 *
 * APP_URL in .env can be set to "auto" — then origin/rpId are derived
 * from the actual HTTP request Host header, which lets the site work
 * from any IP address without reconfiguring .env.
 */

function wa_init(): void {
    static $done = false;
    if ($done) return;
    $done = true;
    $a = __DIR__ . '/../vendor/autoload.php';
    if (file_exists($a)) require_once $a;
}

function b64e(string $s): string {
    return rtrim(strtr(base64_encode($s), '+/', '-_'), '=');
}

function b64d(string $s): string {
    $r = strlen($s) % 4;
    return (string)base64_decode(strtr($s, '-_', '+/') . ($r ? str_repeat('=', 4 - $r) : ''));
}

/**
 * Determine the current request's origin (scheme + host + port).
 * If APP_URL=auto or APP_URL is not set, derives from $_SERVER.
 */
function wa_origin(): string {
    env_load();
    $configured = getenv('APP_URL') ?: 'auto';

    if ($configured !== 'auto' && $configured !== '') {
        return rtrim($configured, '/');
    }

    // Auto-detect from request
    $scheme = (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off') ? 'https' : 'http';
    $host   = $_SERVER['HTTP_HOST'] ?? $_SERVER['SERVER_NAME'] ?? 'localhost';
    return $scheme . '://' . $host;
}

function wa_rp_id(): string {
    $origin = wa_origin();
    $host   = (string)(parse_url($origin, PHP_URL_HOST) ?: 'localhost');
    // Strip port from host for rpId
    return preg_replace('/:\d+$/', '', $host);
}

function wa_origin_ok(string $actual): bool {
    $expected = wa_origin();
    if (rtrim($actual, '/') === rtrim($expected, '/')) return true;
    // Allow any localhost variant
    $h = parse_url($actual, PHP_URL_HOST) ?: '';
    return in_array($h, ['localhost', '127.0.0.1', '::1'], true);
}

function wa_user_creds(int $uid): array {
    $st = db()->prepare('SELECT credential_id FROM webauthn_credentials WHERE user_id=?');
    $st->execute([$uid]);
    return array_map(fn($r) => ['id' => $r['credential_id'], 'type' => 'public-key'], $st->fetchAll());
}

// ─── CBOR ─────────────────────────────────────────────────────────────────

function wa_cbor(string $bytes): mixed {
    wa_init();
    if (class_exists(\CBOR\Decoder::class)) {
        try {
            return \CBOR\Decoder::create()->decode(\CBOR\StringStream::create($bytes))->normalize();
        } catch (\Throwable $e) {
            throw new \RuntimeException('CBOR(fw): ' . $e->getMessage());
        }
    }
    $o = 0;
    return _cbor($bytes, $o);
}

function _cbor(string $d, int &$o): mixed {
    if ($o >= strlen($d)) throw new \RuntimeException('CBOR EOF');
    $b = ord($d[$o++]); $mt = $b >> 5; $ai = $b & 0x1f;
    $v = match(true) {
        $ai < 24   => $ai,
        $ai === 24 => ord($d[$o++]),
        $ai === 25 => (function() use ($d, &$o) { $r = unpack('n', substr($d, $o, 2))[1]; $o += 2; return $r; })(),
        $ai === 26 => (function() use ($d, &$o) { $r = unpack('N', substr($d, $o, 4))[1]; $o += 4; return $r; })(),
        $ai === 27 => (function() use ($d, &$o) {
            // 64-bit unsigned int — read as two 32-bit halves (PHP int is 64-bit on 64-bit systems)
            $hi = unpack('N', substr($d, $o, 4))[1]; $o += 4;
            $lo = unpack('N', substr($d, $o, 4))[1]; $o += 4;
            return ($hi << 32) | $lo;
        })(),
        // ai=31 = indefinite length — not used in WebAuthn keys, skip gracefully
        $ai === 31 => 0,
        default    => throw new \RuntimeException("CBOR ai=$ai unsupported"),
    };
    return match($mt) {
        0 => $v,
        1 => -1 - $v,
        2 => (function() use ($d, &$o, $v) { $s = substr($d, $o, $v); $o += $v; return $s; })(),
        3 => (function() use ($d, &$o, $v) { $s = substr($d, $o, $v); $o += $v; return $s; })(),
        4 => (function() use ($d, &$o, $v) { $a = []; for ($i = 0; $i < $v; $i++) $a[] = _cbor($d, $o); return $a; })(),
        5 => (function() use ($d, &$o, $v) {
            $m = [];
            for ($i = 0; $i < $v; $i++) { $k = _cbor($d, $o); $m[$k] = _cbor($d, $o); }
            return $m;
        })(),
        default => throw new \RuntimeException("CBOR mt=$mt unsupported"),
    };
}

// ─── Signature verification ───────────────────────────────────────────────

function wa_verify(string $cborKey, string $data, string $sig): bool {
    wa_init();
    $km  = wa_cbor($cborKey);
    if (!is_array($km)) return false;
    $alg = (int)($km[3] ?? -7);

    if ($alg === -7 && class_exists(\Cose\Key\Ec2Key::class)) {
        try {
            $key = \Cose\Key\Ec2Key::create([
                \Cose\Key\Ec2Key::DATA_CURVE => \Cose\Key\Ec2Key::CURVE_P256,
                \Cose\Key\Ec2Key::DATA_X     => $km[-2] ?? '',
                \Cose\Key\Ec2Key::DATA_Y     => $km[-3] ?? '',
            ]);
            return (new \Cose\Algorithm\Signature\ECDSA\ES256())->verify($data, $key, $sig);
        } catch (\Throwable) {}
    }
    if ($alg === -257 && class_exists(\Cose\Key\RsaKey::class)) {
        try {
            $key = \Cose\Key\RsaKey::create([
                \Cose\Key\RsaKey::DATA_N => $km[-1] ?? '',
                \Cose\Key\RsaKey::DATA_E => $km[-2] ?? '',
            ]);
            return (new \Cose\Algorithm\Signature\RSA\RS256())->verify($data, $key, $sig);
        } catch (\Throwable) {}
    }
    return wa_openssl($km, $alg, $data, $sig);
}

function wa_openssl(array $km, int $alg, string $data, string $sig): bool {
    try {
        if ($alg === -7) {
            $x = $km[-2] ?? ''; $y = $km[-3] ?? '';
            if (!$x || !$y) return false;
            $pem = wa_ec_pem("\x04" . $x . $y);
        } elseif ($alg === -257) {
            $n = $km[-1] ?? ''; $e = $km[-2] ?? '';
            if (!$n || !$e) return false;
            $pem = wa_rsa_pem($n, $e);
        } else {
            return false;
        }
        $pub = openssl_pkey_get_public($pem);
        return $pub && openssl_verify($data, $sig, $pub, OPENSSL_ALGO_SHA256) === 1;
    } catch (\Throwable) { return false; }
}

function wa_ec_pem(string $point): string {
    $oid = "\x30\x13\x06\x07\x2a\x86\x48\xce\x3d\x02\x01\x06\x08\x2a\x86\x48\xce\x3d\x03\x01\x07";
    $bs  = "\x03" . wa_dlen(strlen($point) + 1) . "\x00" . $point;
    $seq = "\x30" . wa_dlen(strlen($oid) + strlen($bs)) . $oid . $bs;
    return "-----BEGIN PUBLIC KEY-----\n" . chunk_split(base64_encode($seq), 64, "\n") . "-----END PUBLIC KEY-----\n";
}

function wa_rsa_pem(string $n, string $e): string {
    $en = wa_dint($n); $ee = wa_dint($e);
    $sq = "\x30" . wa_dlen(strlen($en) + strlen($ee)) . $en . $ee;
    $oi = "\x30\x0d\x06\x09\x2a\x86\x48\x86\xf7\x0d\x01\x01\x01\x05\x00";
    $bs = "\x03" . wa_dlen(strlen($sq) + 1) . "\x00" . $sq;
    $pk = "\x30" . wa_dlen(strlen($oi) + strlen($bs)) . $oi . $bs;
    return "-----BEGIN PUBLIC KEY-----\n" . chunk_split(base64_encode($pk), 64, "\n") . "-----END PUBLIC KEY-----\n";
}

function wa_dint(string $b): string {
    $b = ltrim($b, "\x00") ?: "\x00";
    if (ord($b[0]) > 0x7f) $b = "\x00" . $b;
    return "\x02" . wa_dlen(strlen($b)) . $b;
}

function wa_dlen(int $n): string {
    if ($n < 128) return chr($n);
    $e = ''; for ($x = $n; $x > 0; $x >>= 8) $e = chr($x & 0xff) . $e;
    return chr(0x80 | strlen($e)) . $e;
}

// ─── Registration ─────────────────────────────────────────────────────────

function webauthn_registration_options(array $user): array {
    session_init();
    $ch = random_bytes(32);
    $_SESSION['wa_reg_ch'] = b64e($ch);
    return [
        'rp'               => ['name' => '1xVKA', 'id' => wa_rp_id()],
        'user'             => [
            'id'          => b64e(pack('N', (int)$user['id'])),
            'name'        => $user['username'],
            'displayName' => $user['username'],
        ],
        'challenge'        => b64e($ch),
        'pubKeyCredParams' => [
            ['alg' => -7,   'type' => 'public-key'],
            ['alg' => -257, 'type' => 'public-key'],
        ],
        'timeout'          => 60000,
        'attestation'      => 'none',
        'authenticatorSelection' => ['userVerification' => 'preferred', 'residentKey' => 'preferred'],
        'excludeCredentials' => wa_user_creds((int)$user['id']),
    ];
}

function webauthn_verify_registration(array $user, array $data): array {
    session_init();
    $expectedCh = $_SESSION['wa_reg_ch'] ?? '';
    unset($_SESSION['wa_reg_ch']);
    try {
        if (empty($data['response']['clientDataJSON']) || empty($data['response']['attestationObject']))
            return ['ok' => false, 'error' => 'Неполные данные'];

        $cdRaw = b64d($data['response']['clientDataJSON']);
        $cd    = json_decode($cdRaw, true, 512, JSON_THROW_ON_ERROR);

        if (($cd['type'] ?? '') !== 'webauthn.create')
            return ['ok' => false, 'error' => 'Неверный тип'];
        if (!hash_equals($expectedCh, $cd['challenge'] ?? ''))
            return ['ok' => false, 'error' => 'Неверный challenge'];
        if (!wa_origin_ok($cd['origin'] ?? ''))
            return ['ok' => false, 'error' => 'Неверный origin: ' . ($cd['origin'] ?? '') . ' (ожидается: ' . wa_origin() . ')'];

        $attObj = wa_cbor(b64d($data['response']['attestationObject']));
        if (!is_array($attObj) || !isset($attObj['authData']))
            return ['ok' => false, 'error' => 'Нет authData'];

        $ad = (string)$attObj['authData'];
        if (strlen($ad) < 55)
            return ['ok' => false, 'error' => 'authData слишком короткий'];
        if (!hash_equals(hash('sha256', wa_rp_id(), true), substr($ad, 0, 32)))
            return ['ok' => false, 'error' => 'RP ID hash не совпадает. Убедитесь что APP_URL в .env содержит правильный хост'];

        $flags = ord($ad[32]);
        if (!($flags & 0x01)) return ['ok' => false, 'error' => 'UP flag не установлен'];
        if (!($flags & 0x40)) return ['ok' => false, 'error' => 'AT flag не установлен'];

        $credIdLen = unpack('n', substr($ad, 53, 2))[1];
        $credId    = substr($ad, 55, $credIdLen);
        $cborKey   = substr($ad, 55 + $credIdLen);
        if (!$credId || !$cborKey)
            return ['ok' => false, 'error' => 'Пустой credId или publicKey'];

        $cid = b64e($credId);
        $dup = db()->prepare('SELECT id FROM webauthn_credentials WHERE credential_id=?');
        $dup->execute([$cid]);
        if ($dup->fetch()) return ['ok' => false, 'error' => 'Ключ уже зарегистрирован'];

        db()->prepare('INSERT INTO webauthn_credentials (user_id,credential_id,public_key,sign_count) VALUES (?,?,?,0)')
           ->execute([(int)$user['id'], $cid, b64e($cborKey)]);
        db()->prepare('UPDATE users SET webauthn_enabled=1 WHERE id=?')
           ->execute([(int)$user['id']]);
        return ['ok' => true];

    } catch (\JsonException $e) { return ['ok' => false, 'error' => 'JSON: ' . $e->getMessage()];
    } catch (\Throwable $e)     { return ['ok' => false, 'error' => 'Ошибка: ' . $e->getMessage()]; }
}

// ─── Authentication ───────────────────────────────────────────────────────

function webauthn_auth_options(int $uid): array {
    session_init();
    $ch = random_bytes(32);
    $_SESSION['wa_auth_ch']  = b64e($ch);
    $_SESSION['wa_auth_uid'] = $uid;
    return [
        'challenge'        => b64e($ch),
        'rpId'             => wa_rp_id(),
        'timeout'          => 60000,
        'userVerification' => 'preferred',
        'allowCredentials' => wa_user_creds($uid),
    ];
}

function webauthn_verify_auth(array $data): array {
    session_init();
    $expectedCh = $_SESSION['wa_auth_ch']  ?? '';
    $uid        = (int)($_SESSION['wa_auth_uid'] ?? $_SESSION['wa_pending'] ?? 0);
    unset($_SESSION['wa_auth_ch'], $_SESSION['wa_auth_uid']);
    if (!$uid) return ['ok' => false, 'error' => 'Сессия не найдена'];

    try {
        $cdRaw = b64d($data['response']['clientDataJSON'] ?? '');
        $cd    = json_decode($cdRaw, true, 512, JSON_THROW_ON_ERROR);

        if (($cd['type'] ?? '') !== 'webauthn.get')
            return ['ok' => false, 'error' => 'Неверный тип'];
        if (!hash_equals($expectedCh, $cd['challenge'] ?? ''))
            return ['ok' => false, 'error' => 'Неверный challenge'];
        if (!wa_origin_ok($cd['origin'] ?? ''))
            return ['ok' => false, 'error' => 'Неверный origin'];

        $cid = b64e(b64d($data['id'] ?? ''));
        $st  = db()->prepare('SELECT * FROM webauthn_credentials WHERE credential_id=? AND user_id=?');
        $st->execute([$cid, $uid]);
        $cred = $st->fetch();
        if (!$cred) return ['ok' => false, 'error' => 'Ключ не найден'];

        $ad = b64d($data['response']['authenticatorData'] ?? '');
        if (strlen($ad) < 37) return ['ok' => false, 'error' => 'authenticatorData слишком короткий'];
        if (!hash_equals(hash('sha256', wa_rp_id(), true), substr($ad, 0, 32)))
            return ['ok' => false, 'error' => 'RP ID не совпадает'];
        if (!(ord($ad[32]) & 0x01))
            return ['ok' => false, 'error' => 'UP flag не установлен'];

        $signedData = $ad . hash('sha256', $cdRaw, true);
        $sig        = b64d($data['response']['signature'] ?? '');
        if (!wa_verify(b64d($cred['public_key']), $signedData, $sig))
            return ['ok' => false, 'error' => 'Неверная подпись'];

        $newCnt = unpack('N', substr($ad, 33, 4))[1];
        $oldCnt = (int)$cred['sign_count'];
        if ($newCnt > 0 && $newCnt <= $oldCnt)
            return ['ok' => false, 'error' => 'Replay attack detected'];

        db()->prepare('UPDATE webauthn_credentials SET sign_count=? WHERE id=?')
           ->execute([$newCnt, $cred['id']]);
        return ['ok' => true, 'user_id' => $uid];

    } catch (\Throwable $e) { return ['ok' => false, 'error' => 'Ошибка: ' . $e->getMessage()]; }
}

// ─── Admin WebAuthn (.env-based, no DB) ───────────────────────────────────

/**
 * Generate challenge for admin WebAuthn auth.
 * ADMIN_CREDENTIAL_ID and ADMIN_PUBLIC_KEY must be set in .env beforehand.
 */
function admin_wa_auth_options(): array {
    session_init();
    env_load();

    $credId = getenv('ADMIN_CREDENTIAL_ID') ?: '';
    if (!$credId)
        throw new \RuntimeException('ADMIN_CREDENTIAL_ID не задан в .env');

    $ch = random_bytes(32);
    $_SESSION['admin_wa_ch'] = b64e($ch);

    return [
        'challenge'        => b64e($ch),
        'rpId'             => wa_rp_id(),
        'timeout'          => 60000,
        'userVerification' => 'required',
        'allowCredentials' => [
            ['id' => $credId, 'type' => 'public-key'],
        ],
    ];
}

/**
 * Verify admin WebAuthn assertion against ADMIN_PUBLIC_KEY from .env.
 * No DB involved — credential_id and public_key come entirely from .env.
 */
function admin_wa_verify(array $data): array {
    session_init();
    env_load();

    $expectedCh = $_SESSION['admin_wa_ch'] ?? '';
    unset($_SESSION['admin_wa_ch']);

    if (!$expectedCh)
        return ['ok'=>false,'error'=>'Нет активного challenge'];

    $credId    = getenv('ADMIN_CREDENTIAL_ID') ?: '';
    $pubKeyB64 = getenv('ADMIN_PUBLIC_KEY')    ?: '';

    if (!$credId || !$pubKeyB64)
        return ['ok'=>false,'error'=>'ADMIN_CREDENTIAL_ID / ADMIN_PUBLIC_KEY не заданы в .env'];

    try {
        // 1. Verify clientDataJSON
        $cdRaw = b64d($data['response']['clientDataJSON'] ?? '');
        $cd    = json_decode($cdRaw, true, 512, JSON_THROW_ON_ERROR);

        if (($cd['type'] ?? '') !== 'webauthn.get')
            return ['ok'=>false,'error'=>'Неверный тип clientData'];
        if (!hash_equals($expectedCh, $cd['challenge'] ?? ''))
            return ['ok'=>false,'error'=>'Неверный challenge'];
        if (!wa_origin_ok($cd['origin'] ?? ''))
            return ['ok'=>false,'error'=>'Неверный origin: ' . ($cd['origin'] ?? '')];

        // 2. Verify credential ID matches .env
        // Re-encode to normalize any base64url padding differences
        $incoming = b64e(b64d($data['id'] ?? ''));
        $expected = b64e(b64d($credId));
        if (!hash_equals($expected, $incoming))
            return ['ok'=>false,'error'=>'Ключ не совпадает с ADMIN_CREDENTIAL_ID в .env'];

        // 3. Verify authenticatorData
        $ad = b64d($data['response']['authenticatorData'] ?? '');
        if (strlen($ad) < 37)
            return ['ok'=>false,'error'=>'authenticatorData слишком короткий'];
        if (!hash_equals(hash('sha256', wa_rp_id(), true), substr($ad, 0, 32)))
            return ['ok'=>false,'error'=>'RP ID не совпадает'];
        if (!(ord($ad[32]) & 0x01))
            return ['ok'=>false,'error'=>'UP flag не установлен'];

        // 4. Verify cryptographic signature using public key from .env
        $signedData = $ad . hash('sha256', $cdRaw, true);
        $sig        = b64d($data['response']['signature'] ?? '');
        $cborKey    = b64d($pubKeyB64);

        if (!wa_verify($cborKey, $signedData, $sig))
            return ['ok'=>false,'error'=>'Неверная подпись — проверьте ADMIN_PUBLIC_KEY в .env'];

        return ['ok'=>true];

    } catch (\Throwable $e) {
        return ['ok'=>false,'error'=>'Ошибка верификации: ' . $e->getMessage()];
    }
}
