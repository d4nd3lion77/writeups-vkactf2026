<?php
declare(strict_types=1);

session_name('WLAB');
session_set_cookie_params(['lifetime'=>0,'path'=>'/','httponly'=>true,'samesite'=>'Lax']);
session_start();

require_once __DIR__ . '/Auth.php';
$config = require __DIR__ . '/config.php';
$auth   = new Auth($config);

$uri = strtok($_SERVER['REQUEST_URI'] ?? '/', '?');
if ($uri === '/api') {
    header('Content-Type: application/json; charset=utf-8');
    $action = $_GET['action'] ?? '';
    $body   = json_decode(file_get_contents('php://input'), true) ?? [];

    try {
        match ($action) {
            'login'    => apiLogin($auth, $body),
            'logout'   => apiLogout($auth),
            'generate' => apiGenerate($auth, $body),
            'verify'   => apiVerify($auth, $body),
            'debug'    => apiDebug($auth),
            default    => jsonErr(404, "Unknown action: $action"),
        };
    } catch (Throwable $e) {
        jsonErr(500, $e->getMessage());
    }
    exit;
}

$isAuth = $auth->isAuthenticated();
$user   = $_SESSION['user'] ?? '';
?>
<!DOCTYPE html>
<html lang="ru">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>WebAuthn Lab</title>
<style>
*,*::before,*::after{box-sizing:border-box;margin:0;padding:0}
:root{
  --bg:#0f1117;--bg2:#1a1d27;--bg3:#22263a;
  --border:rgba(255,255,255,.1);--text:#e2e8f0;--muted:#8892a4;
  --accent:#7c6af7;--ah:#9b8dff;--green:#34d399;--red:#f87171;
  --r:10px;--mono:'JetBrains Mono','Fira Code',monospace
}
body{font-family:system-ui,sans-serif;background:var(--bg);color:var(--text);min-height:100vh;display:flex;flex-direction:column}
header{background:var(--bg2);border-bottom:1px solid var(--border);padding:0 24px;height:56px;display:flex;align-items:center;justify-content:space-between}
.logo{font-size:16px;font-weight:600}.logo span{color:var(--accent)}
.badge{font-size:11px;padding:2px 8px;border-radius:20px;font-weight:500}
.ok{background:rgba(52,211,153,.15);color:var(--green)}
.no{background:rgba(248,113,113,.15);color:var(--red)}
main{flex:1;max-width:900px;width:100%;margin:32px auto;padding:0 20px;display:flex;flex-direction:column;gap:20px}
.card{background:var(--bg2);border:1px solid var(--border);border-radius:var(--r);overflow:hidden}
.card-header{padding:14px 20px;background:var(--bg3);border-bottom:1px solid var(--border);font-size:13px;font-weight:600;color:var(--muted);letter-spacing:.5px;text-transform:uppercase}
.card-body{padding:20px;display:flex;flex-direction:column;gap:16px}
label{font-size:13px;color:var(--muted);display:block;margin-bottom:5px}
input,textarea{width:100%;background:var(--bg);border:1px solid var(--border);border-radius:7px;color:var(--text);padding:9px 12px;font-size:14px;font-family:inherit;outline:none;transition:border-color .15s}
input:focus,textarea:focus{border-color:var(--accent)}
textarea{resize:vertical;font-family:var(--mono);font-size:12px;min-height:72px}
.field-row{display:grid;grid-template-columns:1fr 1fr;gap:12px}
.copy-row{display:flex;gap:8px}
.copy-row input{flex:1}
button{background:var(--accent);color:#fff;border:none;border-radius:7px;padding:9px 20px;font-size:14px;font-weight:500;cursor:pointer;transition:background .15s}
button:hover{background:var(--ah)}
button.sec{background:transparent;border:1px solid var(--border);color:var(--muted)}
button.sec:hover{border-color:var(--accent);color:var(--text);background:transparent}
.result{background:var(--bg);border:1px solid var(--border);border-radius:7px;padding:12px;font-family:var(--mono);font-size:12px;white-space:pre-wrap;word-break:break-all;max-height:460px;overflow-y:auto;color:var(--green);display:none}
.result.err{color:var(--red)}
.result.show{display:block}
.hint{font-size:12px;color:var(--muted);line-height:1.7}
.hint code{background:var(--bg3);padding:1px 5px;border-radius:4px;font-size:11px}
.sec-label{font-size:11px;font-weight:600;color:var(--accent);letter-spacing:.5px;text-transform:uppercase}
.divider{border:none;border-top:1px solid var(--border)}
.cbtn{background:none;border:1px solid var(--border);color:var(--muted);padding:6px 12px;font-size:12px;border-radius:7px;cursor:pointer;white-space:nowrap}
.cbtn:hover{color:var(--text);border-color:var(--accent);background:none}
footer{text-align:center;padding:20px;font-size:12px;color:var(--muted);border-top:1px solid var(--border)}
</style>
</head>
<body>
<header>
  <div class="logo">WebAuthn <span>Lab</span></div>
  <?php if ($isAuth): ?>
    <div style="display:flex;align-items:center;gap:12px">
      <span style="font-size:13px;color:var(--muted)"><?=htmlspecialchars($user)?></span>
      <span class="badge ok">● Авторизован</span>
      <button class="sec" style="padding:5px 12px;font-size:12px" onclick="doLogout()">Выйти</button>
    </div>
  <?php else: ?>
    <span class="badge no">● Не авторизован</span>
  <?php endif ?>
</header>

<main>
<?php if (!$isAuth): ?>

<div class="card">
  <div class="card-header">Вход</div>
  <div class="card-body">
    <p class="hint">Введите логин и пароль.</p>
    <div class="field-row">
      <div><label>Логин</label><input id="u" type="text" autocomplete="username" placeholder="admin"></div>
      <div><label>Пароль</label><input id="p" type="password" autocomplete="current-password" placeholder="••••••"></div>
    </div>
    <div><button onclick="doLogin()">Войти</button></div>
    <div id="lr" class="result"></div>
  </div>
</div>

<?php else: ?>

<div class="card">
  <div class="card-header">Генерация WebAuthn payload + подпись</div>
  <div class="card-body">
    <p class="hint">
      Введите <code>challenge</code> и <code>sign count</code> — сервер соберёт
      <code>authenticatorData&nbsp;||&nbsp;SHA-256(clientDataJSON)</code>
      и сразу подпишет приватным ключом из <code>.env</code>.
    </p>

    <div class="field-row">
      <div>
        <label>Challenge</label>
        <div class="copy-row">
          <input id="challenge" type="text" placeholder="случайная строка">
          <button class="cbtn" onclick="rnd()">random</button>
        </div>
      </div>
      <div>
        <label>Sign count</label>
        <input id="signcount" type="number" value="1" min="0">
      </div>
    </div>

    <div><button onclick="doGenerate()">Сгенерировать и подписать</button></div>

    <div id="result-section" style="display:none;flex-direction:column;gap:14px">
      <hr class="divider">
      <div class="sec-label">Результат</div>

      <div>
        <label>clientDataJSON</label>
        <textarea id="out-cdj" readonly rows="3"></textarea>
      </div>
      <div>
        <label>authenticatorData (base64url) — rpIdHash + flags + signCount</label>
        <div class="copy-row"><input id="out-ad" readonly><button class="cbtn" onclick="cp('out-ad',this)">копировать</button></div>
      </div>
      <div>
        <label>dataToSign (base64url) — authenticatorData || SHA-256(clientDataJSON)</label>
        <div class="copy-row"><input id="out-dts" readonly><button class="cbtn" onclick="cp('out-dts',this)">копировать</button></div>
      </div>
      <div>
        <label>Подпись (base64url) — ECDSA P-256 / SHA-256</label>
        <div class="copy-row"><input id="out-sig" readonly><button class="cbtn" onclick="cp('out-sig',this)">копировать</button></div>
      </div>
    </div>

    <div id="gr" class="result"></div>
  </div>
</div>

<div class="card">
  <div class="card-header">Верификация подписи</div>
  <div class="card-body">
    <p class="hint">Проверяет подпись публичным ключом из <code>config.php</code>. Используйте данные из блока выше или введите свои.</p>
    <div>
      <label>dataToSign (base64url)</label>
      <input id="vd" type="text" placeholder="data_to_sign_b64">
    </div>
    <div>
      <label>Подпись (base64url)</label>
      <input id="vs" type="text" placeholder="signature">
    </div>
    <div><button onclick="doVerify()">Верифицировать</button></div>
    <div id="vr" class="result"></div>
  </div>
</div>

<?php endif ?>
</main>

<footer>WebAuthn Lab · PHP <?=PHP_VERSION?> · порт 8000</footer>

<script>
const api = (action, body) =>
  fetch('/api?action=' + action, {
    method: 'POST',
    headers: {'Content-Type':'application/json'},
    credentials: 'same-origin',
    body: JSON.stringify(body)
  }).then(r => r.json());

function show(id, data, err=false){
  const el=document.getElementById(id);
  el.className='result show'+(err?' err':'');
  el.textContent=typeof data==='string'?data:JSON.stringify(data,null,2);
}

async function doLogin(){
  const u=document.getElementById('u').value.trim();
  const p=document.getElementById('p').value;
  const res=await api('login',{username:u,password:p});
  if(res.ok) location.reload();
  else show('lr',res.error,true);
}

async function doLogout(){
  await api('logout',{});
  location.reload();
}

async function doGenerate(){
  const challenge  = document.getElementById('challenge').value.trim();
  const sign_count = parseInt(document.getElementById('signcount').value)||0;
  if(!challenge){ show('gr','Введите challenge',true); return; }

  const res = await api('generate', {challenge, sign_count});
  if(!res.ok){ show('gr', res.error, true); return; }

  document.getElementById('out-cdj').value = res.client_data_json;
  document.getElementById('out-ad').value  = res.authenticator_data_b64;
  document.getElementById('out-dts').value = res.data_to_sign_b64;
  document.getElementById('out-sig').value = res.signature;
  document.getElementById('result-section').style.display = 'flex';
  document.getElementById('gr').className = 'result';


  document.getElementById('vd').value = res.data_to_sign_b64;
  document.getElementById('vs').value = res.signature;
}

async function doVerify(){
  const data = document.getElementById('vd').value.trim();
  const sig  = document.getElementById('vs').value.trim();
  if(!data||!sig){ show('vr','Заполните оба поля',true); return; }
  const res = await api('verify',{data,signature:sig});
  show('vr', res, !res.ok||!res.valid);
}

function rnd(){
  const a=new Uint8Array(32); crypto.getRandomValues(a);
  document.getElementById('challenge').value =
    Array.from(a).map(b=>b.toString(16).padStart(2,'0')).join('');
}

function cp(id,btn){
  navigator.clipboard.writeText(document.getElementById(id).value).then(()=>{
    const old=btn.textContent; btn.textContent='скопировано!';
    setTimeout(()=>btn.textContent=old,1500);
  });
}

document.addEventListener('keydown',e=>{
  if(e.key==='Enter'&&document.activeElement?.id==='p') doLogin();
});
</script>
</body>
</html>
<?php


function apiLogin(Auth $auth, array $body): void {
    $u = trim($body['username'] ?? '');
    $p = $body['password'] ?? '';
    if ($u===''||$p==='') { jsonErr(400,'Заполните логин и пароль'); }
    if ($auth->login($u,$p)) { jsonOk(['message'=>'OK','user'=>$u]); }
    else { jsonErr(401,'Неверный логин или пароль'); }
}

function apiLogout(Auth $auth): void {
    $auth->logout();
    jsonOk(['message'=>'Сессия завершена']);
}

function apiGenerate(Auth $auth, array $body): void {
    if (!$auth->isAuthenticated()) { jsonErr(401,'Требуется авторизация'); }
    $challenge = trim($body['challenge'] ?? '');
    $signCount = max(0, (int)($body['sign_count'] ?? 0));
    if ($challenge==='') { jsonErr(400,'Поле challenge обязательно'); }
    jsonOk($auth->generateAndSign($challenge, $signCount));
}

function apiVerify(Auth $auth, array $body): void {
    if (!$auth->isAuthenticated()) { jsonErr(401,'Требуется авторизация'); }
    $data = $body['data'] ?? '';
    $sig  = $body['signature'] ?? '';
    if ($data===''||$sig==='') { jsonErr(400,'Нужны data и signature'); }
    $valid = $auth->verify($data, $sig);
    jsonOk(['valid'=>$valid,'message'=>$valid?'Подпись корректна':'Неверная подпись']);
}

function apiDebug(Auth $auth): void {
    jsonOk(['session'=>$_SESSION,'authenticated'=>$auth->isAuthenticated()]);
}

function jsonOk(array $data): never {
    echo json_encode(['ok'=>true,...$data], JSON_UNESCAPED_UNICODE|JSON_PRETTY_PRINT);
    exit;
}
function jsonErr(int $code, string $msg): never {
    http_response_code($code);
    echo json_encode(['ok'=>false,'error'=>$msg], JSON_UNESCAPED_UNICODE);
    exit;
}
