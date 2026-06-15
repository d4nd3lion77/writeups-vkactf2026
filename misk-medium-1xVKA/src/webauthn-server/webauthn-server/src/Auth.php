<?php
declare(strict_types=1);

class Auth
{
    private array $config;

    public function __construct(array $config)
    {
        $this->config = $config;
    }


    public function isAuthenticated(): bool
    {
        return isset($_SESSION['auth']) && $_SESSION['auth'] === true;
    }

    public function login(string $username, string $password): bool
    {
        $cfg = $this->config['auth'];
        if (hash_equals($cfg['username'], $username) && hash_equals($cfg['password'], $password)) {
            $_SESSION['auth'] = true;
            $_SESSION['user'] = $username;
            return true;
        }
        return false;
    }

    public function logout(): void
    {
        $_SESSION = [];
        session_destroy();
    }

    public function generateAndSign(string $challenge, int $signCount): array
    {
        $cfg = $this->config['webauthn'];

        $clientData = json_encode([
            'type'        => 'webauthn.get',
            'challenge'   => $challenge,
            'origin'      => $cfg['origin'],
            'crossOrigin' => false,
        ], JSON_UNESCAPED_SLASHES | JSON_UNESCAPED_UNICODE);

        $rpIdHash          = hash('sha256', $cfg['rp_id'], true);
        $flags             = 0x01; // UP — user present
        $authenticatorData = $rpIdHash . chr($flags) . pack('N', $signCount);

        $dataToSign = $authenticatorData . hash('sha256', $clientData, true);

        $signature = $this->sign($dataToSign);

        return [
            'challenge'              => $challenge,
            'sign_count'             => $signCount,
            'client_data_json'       => $clientData,
            'client_data_b64'        => $this->base64urlEncode($clientData),
            'authenticator_data_b64' => $this->base64urlEncode($authenticatorData),
            'data_to_sign_b64'       => $this->base64urlEncode($dataToSign),
            'signature'              => $signature,
            'algorithm'              => 'ECDSA P-256 / SHA-256',
        ];
    }

    public function verify(string $dataB64, string $signatureB64): bool
    {
        $pubKey = openssl_pkey_get_public($this->config['public_key']);
        if ($pubKey === false) return false;
        $data      = $this->base64urlDecode($dataB64);
        $signature = $this->base64urlDecode($signatureB64);
        return openssl_verify($data, $signature, $pubKey, OPENSSL_ALGO_SHA256) === 1;
    }

    private function sign(string $rawData): string
    {
        $rawKey = str_replace('\n', "\n", $_ENV['PRIVATE_KEY'] ?? getenv('PRIVATE_KEY') ?: '');
        $key    = openssl_pkey_get_private($rawKey);
        if ($key === false) {
            throw new \RuntimeException('Не удалось загрузить PRIVATE_KEY из .env');
        }
        openssl_sign($rawData, $signature, $key, OPENSSL_ALGO_SHA256);
        return $this->base64urlEncode($signature);
    }

    public function base64urlEncode(string $data): string
    {
        return rtrim(strtr(base64_encode($data), '+/', '-_'), '=');
    }

    public function base64urlDecode(string $data): string
    {
        return base64_decode(strtr($data, '-_', '+/') . str_repeat('=', (4 - strlen($data) % 4) % 4));
    }
}
