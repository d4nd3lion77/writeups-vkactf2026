import json
import hashlib
import struct
import base64
import requests
from urllib.parse import urlparse
from cryptography.hazmat.primitives.asymmetric.ec import (
    ECDSA, EllipticCurvePrivateKey
)
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.backends import default_backend



BASE_URL = "http://localhost:31200"          

ADMIN_USERNAME = "admin"
ADMIN_PASSWORD = "Admin@OlympBet2026"       

ADMIN_CREDENTIAL_ID = "AQIDBAUGBwgJCgsMDQ4PEBESExQVFhcYGRobHB0eHw"                    

webauthn_url='http://localhost:31002'

def Login(session: requests.Session, base_url: str, username: str, password: str) -> bool:
    resp = session.post(
        f"{base_url}/api?action=login",
        json={"username": username, "password": password},
    )
    data = resp.json()
    if data.get("ok"):
        print(f"[+] Авторизован как '{data.get('user')}'")
        return True
    print(f"[-] Ошибка авторизации: {data.get('error')}")
    return False


def generate(session: requests.Session, base_url: str, challenge: str, sign_count: int) -> dict:
    resp = session.post(
        f"{base_url}/api?action=generate",
        json={"challenge": challenge, "sign_count": sign_count},
    )
    return resp.json()


def build_authenticator_data(rp_id: str) -> bytes:
    rp_id_hash = hashlib.sha256(rp_id.encode()).digest()
    flags      = bytes([0x05])           # UP + UV
    sign_count = struct.pack(">I", 0)    # big-endian uint32
    return rp_id_hash + flags + sign_count


def build_client_data(challenge_b64url: str, origin: str) -> bytes:
    client_data = {
        "type":      "webauthn.get",
        "challenge": challenge_b64url,
        "origin":    origin,
        "crossOrigin": False,
    }
    return json.dumps(client_data, separators=(",", ":")).encode()



def get_origin(base_url: str) -> str:
    parsed = urlparse(base_url)
    return f"{parsed.scheme}://{parsed.netloc}"


def get_rp_id(base_url: str) -> str:
    parsed = urlparse(base_url)
    host   = parsed.hostname or "localhost"
    return host


class AdminSession:
    def __init__(self, base_url: str = BASE_URL):
        self.base_url = base_url.rstrip("/")
        self.session  = requests.Session()
        self.session.headers["User-Agent"] = "1xVKA-AdminBot"
        self._logged_in = False

    def login(
        self,
        username:      str = ADMIN_USERNAME,
        password:      str = ADMIN_PASSWORD,
        credential_id: str = ADMIN_CREDENTIAL_ID,
        count: int = 1
    ) -> "AdminSession":

        print(f"[*] Подключение к {self.base_url}")

        self._get("/")

        print(f"[*] Шаг 1: проверка пароля для '{username}'")
        csrf  = self._get_csrf()
        resp  = self._post("/?action=admin_login", data={
            "csrf":     csrf,
            "username": username,
            "password": password,
        })
        if not resp.get("ok"):
            raise RuntimeError(f"Ошибка пароля: {resp.get('error')}")
        print("[+] Пароль принят")

        print("[*] Шаг 2: получение WebAuthn challenge")
        options = self._post("/?action=admin_wa_options")
        if options.get("ok") is False:
            raise RuntimeError(f"Ошибка получения challenge: {options.get('error')}")

        challenge_b64 = options["challenge"]
        rp_id         = options.get("rpId") or get_rp_id(self.base_url)
        origin        = get_origin(self.base_url)
        print(f"[*] challenge={challenge_b64[:20]}…  rpId={rp_id}  origin={origin}")


        session = requests.Session()

        Login(session, webauthn_url, ADMIN_USERNAME, ADMIN_PASSWORD)
        print(f"[>] Отправляем: challenge='{challenge_b64}', sign_count={count}")
        result = generate(session, webauthn_url, challenge_b64, count)
        print(result)
        client_data_json=result["client_data_b64"]
        authenticator_data=result["authenticator_data_b64"]
        signature=result["signature"]

        # 5. Отправить assertion на сервер
        print("[*] Шаг 3: отправка подписанного assertion")
        assertion = {
            "id":    credential_id,
            "type":  "public-key",
            "rawId": credential_id,
            "response": {
                "clientDataJSON":    client_data_json,
                "authenticatorData": authenticator_data,
                "signature":         signature,
                "userHandle":        None,
            },
        }

        result = self._post_json("/?action=admin_wa_verify", assertion)
        if not result.get("ok"):
            raise RuntimeError(f"WebAuthn ошибка: {result.get('error')}")

        self._logged_in = True
        print("[+] Вход выполнен! Сессия администратора активна.")
        print(f"[+] Куки: {dict(self.session.cookies)}")
        return True


    def _post(self, path: str, data: dict = None) -> dict:
        resp = self.session.post(self.base_url + path, data=data or {})
        resp.raise_for_status()
        return resp.json()

    def _post_json(self, path: str, payload: dict) -> dict:
        resp = self.session.post(
            self.base_url + path,
            json=payload,
            headers={"Content-Type": "application/json"},
        )
        resp.raise_for_status()
        return resp.json()

    def _get(self, path: str) -> requests.Response:
        return self.session.get(self.base_url + path)


    def _get_csrf(self) -> str:
        """Получить CSRF-токен из куки или загрузить страницу."""
        for name, value in self.session.cookies.items():
            if name.lower() == "csrf":
                return value
        self._get("/")
        # Извлечь из скрытого поля на странице логина
        resp = self._get("/?page=admin_login")
        import re
        match = re.search(r'id="adm-csrf"\s+value="([a-f0-9]+)"', resp.text)
        if match:
            return match.group(1)
        # Если не нашли — вернуть пустую строку (сервер выдаст CSRF ошибку)
        print("[!] Предупреждение: CSRF токен не найден")
        return ""
    

if __name__ == "__main__":
    admin = AdminSession(BASE_URL)
    i=0
    while True:
        if admin.login(
            username       = ADMIN_USERNAME,
            password       = ADMIN_PASSWORD,
            credential_id  = ADMIN_CREDENTIAL_ID,
            count= i
        ):
            break
        i+=1
