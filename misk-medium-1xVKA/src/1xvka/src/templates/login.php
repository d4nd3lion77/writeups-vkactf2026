<?php // login.php ?>
<div class="auth-page">
<div class="auth-card">
  <div class="auth-head"><div class="auth-icon">🔐</div><h1>Войти</h1></div>

  <div id="step-pass">
    <form onsubmit="doLogin(event)">
      <input type="hidden" id="login-csrf" value="<?= htmlspecialchars($csrf) ?>">
      <div class="field"><label>Логин или Email</label><input id="login-l" class="input" type="text" required autocomplete="username"></div>
      <div class="field"><label>Пароль</label><input id="login-p" class="input" type="password" required autocomplete="current-password"></div>
      <div id="login-err" class="err hidden"></div>
      <button id="login-btn" class="btn btn-primary" style="width:100%">Войти</button>
    </form>
  </div>

  <div id="step-2fa" class="hidden" style="text-align:center;padding:16px 0">
    <div style="font-size:48px;margin-bottom:12px">🔑</div>
    <h2>Подтвердите вход</h2>
    <p style="color:var(--t2);margin:8px 0 20px">Используйте ваш ключ безопасности или биометрию</p>
    <div id="twofa-err" class="err hidden" style="margin-bottom:12px"></div>
    <div id="twofa-wait" class="hidden" style="color:var(--t2);font-size:13px;margin-bottom:12px">⏳ Ожидаем устройство…</div>
    <button id="twofa-btn" onclick="doWebAuthnAuth()" class="btn btn-primary" style="width:100%">🔐 Подтвердить</button>
    <button onclick="cancelTwoFA()" class="btn btn-outline" style="width:100%;margin-top:8px">← Назад</button>
  </div>

  <p class="auth-foot">Нет аккаунта? <a href="/?page=register">Зарегистрироваться</a></p>
</div>
</div>

<script>
async function doLogin(e) {
  e.preventDefault();
  const btn = document.getElementById('login-btn');
  const err = document.getElementById('login-err');
  btn.disabled = true; btn.textContent = 'Входим…'; err.classList.add('hidden');
  try {
    const r = await fetch('/?action=login', {method:'POST', body: new URLSearchParams({
      csrf: document.getElementById('login-csrf').value,
      login: document.getElementById('login-l').value.trim(),
      password: document.getElementById('login-p').value,
    })});
    const d = await r.json();
    if (d.ok && d.needs_2fa) {
      document.getElementById('step-pass').classList.add('hidden');
      document.getElementById('step-2fa').classList.remove('hidden');
      setTimeout(doWebAuthnAuth, 400);
    } else if (d.ok) {
      location.href = '/';
    } else {
      err.textContent = d.error; err.classList.remove('hidden');
      btn.disabled = false; btn.textContent = 'Войти';
    }
  } catch(ex) {
    err.textContent = 'Ошибка сети'; err.classList.remove('hidden');
    btn.disabled = false; btn.textContent = 'Войти';
  }
}

async function doWebAuthnAuth() {
  const err  = document.getElementById('twofa-err');
  const wait = document.getElementById('twofa-wait');
  const btn  = document.getElementById('twofa-btn');
  err.classList.add('hidden'); wait.classList.remove('hidden'); btn.disabled = true;

  try {
    const optR = await fetch('/?action=wa_auth_options', {method:'POST'});
    const optT = await optR.text();
    let opts;
    try { opts = JSON.parse(optT); } catch { throw new Error('Сервер: ' + optT.slice(0,150)); }
    if (opts.ok === false) throw new Error(opts.error);

    opts.challenge = b64ToArr(opts.challenge);
    if (opts.allowCredentials) opts.allowCredentials = opts.allowCredentials.map(c=>({...c,id:b64ToArr(c.id)}));

    let asr;
    try { asr = await navigator.credentials.get({publicKey: opts}); }
    catch(e) { throw new Error(e.name === 'NotAllowedError' ? 'Отменено или истекло время' : e.message); }

    const vR = await fetch('/?action=wa_auth_verify', {method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify({
      id: asr.id, type: asr.type, rawId: arrToB64(asr.rawId),
      response: {
        clientDataJSON:    arrToB64(asr.response.clientDataJSON),
        authenticatorData: arrToB64(asr.response.authenticatorData),
        signature:         arrToB64(asr.response.signature),
        userHandle: asr.response.userHandle ? arrToB64(asr.response.userHandle) : null,
      }
    })});
    const vT = await vR.text();
    let res;
    try { res = JSON.parse(vT); } catch { throw new Error('Сервер: ' + vT.slice(0,150)); }
    if (res.ok) { location.href = '/'; }
    else throw new Error(res.error || 'Неверный ключ');
  } catch(e) {
    err.textContent = e.message; err.classList.remove('hidden'); btn.disabled = false;
  } finally { wait.classList.add('hidden'); }
}

function cancelTwoFA() {
  document.getElementById('step-2fa').classList.add('hidden');
  document.getElementById('step-pass').classList.remove('hidden');
  document.getElementById('login-btn').disabled = false;
  document.getElementById('login-btn').textContent = 'Войти';
}
</script>
