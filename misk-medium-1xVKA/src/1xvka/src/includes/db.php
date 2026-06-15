<?php
declare(strict_types=1);

function env_load(): void {
    static $done = false;
    if ($done) return;
    $done = true;
    $paths = ['/var/www/.env', dirname(__DIR__, 2) . '/.env'];
    foreach ($paths as $p) {
        if (!file_exists($p)) continue;
        $auto = __DIR__ . '/../vendor/autoload.php';
        if (!file_exists($auto)) break;
        require_once $auto;
        if (!class_exists(\Dotenv\Dotenv::class)) break;
        try { \Dotenv\Dotenv::createImmutable(dirname($p))->load(); } catch (\Throwable) {}
        break;
    }
}

function db(): PDO {
    static $pdo = null;
    if ($pdo !== null) return $pdo;

    env_load();

    $host    = getenv('DB_HOST') ?: 'mysql';
    $port    = getenv('DB_PORT') ?: '3306';
    $name    = getenv('DB_NAME') ?: '1xvka';
    $user    = getenv('DB_USER') ?: '1xvka';
    $pass    = getenv('DB_PASS') ?: '';
    $dsn     = "mysql:host={$host};port={$port};dbname={$name};charset=utf8mb4";
    $options = [
        PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
        PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
        PDO::ATTR_EMULATE_PREPARES   => false,
    ];

    // Retry loop — PHP-FPM starts before MySQL is fully ready
    $last = null;
    for ($i = 0; $i < 10; $i++) {
        try {
            $pdo = new PDO($dsn, $user, $pass, $options);
            return $pdo;
        } catch (\PDOException $e) {
            $last = $e;
            $pdo  = null;
            sleep(2);
        }
    }

    throw new \RuntimeException('Cannot connect to database: ' . $last->getMessage());
}
