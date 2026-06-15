<?php // admin_login.php ?>
<div class="auth-page">
<div class="auth-card">
  <div class="auth-head">
    <div class="auth-icon">🛡️</div>
    <h1>Панель администратора</h1>
    <p>Пароль + WebAuthn ключ из .env</p>
  </div>

  <!-- Шаг 1: Пароль -->
  <div id="step-pass">
    <form onsubmit="doAdminPass(event)">
      <input type="hidden" id="adm-csrf" value="<?= htmlspecialchars($csrf) ?>">
      <div class="field">
        <label>Логин</label>
        <input id="adm-u" class="input" type="text" required autocomplete="username" placeholder="admin">
      </div>
      <div class="field">
        <label>Пароль</label>
        <input id="adm-p" class="input" type="password" required autocomplete="current-password" placeholder="••••••••">
      </div>
      <div id="pass-err" class="err hidden"></div>
      <button id="pass-btn" class="btn btn-primary" style="width:100%">Далее →</button>
    </form>
  </div>

  <!-- Шаг 2: WebAuthn -->
  <div id="step-wa" class="hidden" style="text-align:center;padding:8px 0">
    <div style="font-size:48px;margin-bottom:12px">🔑</div>
    <h2 style="margin-bottom:8px">Подтвердите личность</h2>
    <p style="color:var(--t2);margin-bottom:20px;font-size:14px">
      Используйте ключ безопасности, Touch ID или Face ID
    </p>
    <div id="wa-err"  class="err hidden" style="margin-bottom:12px;text-align:left"></div>
    <div id="wa-wait" class="hidden" style="color:var(--t2);font-size:13px;margin-bottom:12px">
      ⏳ Ожидаем устройство…
    </div>
    <button id="wa-btn" onclick="doAdminWA()" class="btn btn-primary" style="width:100%">
      🔐 Подтвердить
    </button>
    <button onclick="backToPass()" class="btn btn-outline" style="width:100%;margin-top:8px">
      ← Назад
    </button>
  </div>

  <p class="auth-foot"><a href="/">← На сайт</a></p>
</div>
</div>

<script>
// Шаг 1 — пароль
async function doAdminPass(e) {
  e.preventDefault();
  const btn = document.getElementById('pass-btn');
  const err = document.getElementById('pass-err');
  btn.disabled = true; btn.textContent = 'Проверяем…'; err.classList.add('hidden');

  try {
    const r = await fetch('/?action=admin_login', {
      method: 'POST',
      body: new URLSearchParams({
        csrf:     document.getElementById('adm-csrf').value,
        username: document.getElementById('adm-u').value.trim(),
        password: document.getElementById('adm-p').value,
      })
    });
    const d = await r.json();
    if (d.ok) {
      document.getElementById('step-pass').classList.add('hidden');
      document.getElementById('step-wa').classList.remove('hidden');
      setTimeout(doAdminWA, 300);
    } else {
      err.textContent = d.error;
      err.classList.remove('hidden');
      btn.disabled = false;
      btn.textContent = 'Далее →';
    }
  } catch {
    err.textContent = 'Ошибка сети';
    err.classList.remove('hidden');
    btn.disabled = false;
    btn.textContent = 'Далее →';
  }
}

// Шаг 2 — WebAuthn
async function doAdminWA() {
  const err  = document.getElementById('wa-err');
  const wait = document.getElementById('wa-wait');
  const btn  = document.getElementById('wa-btn');
  err.classList.add('hidden'); wait.classList.remove('hidden'); btn.disabled = true;

  try {
    // Получить challenge от сервера
    const optR = await fetch('/?action=admin_wa_options', {method: 'POST'});
    const optT = await optR.text();
    let opts;
    try { opts = JSON.parse(optT); } catch { throw new Error('Сервер: ' + optT.slice(0, 200)); }
    if (opts.ok === false) throw new Error(opts.error);

    opts.challenge = b64ToArr(opts.challenge);
    if (opts.allowCredentials)
      opts.allowCredentials = opts.allowCredentials.map(c => ({...c, id: b64ToArr(c.id)}));

    // Запрос подтверждения у устройства
    let asr;
    try {
      asr = await navigator.credentials.get({publicKey: opts});
    } catch(e) {
      throw new Error(e.name === 'NotAllowedError' ? 'Отменено или истекло время' : e.message);
    }

    // Верификация на сервере
    const vR = await fetch('/?action=admin_wa_verify', {
      method:  'POST',
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify({
        id:    asr.id,
        type:  asr.type,
        rawId: arrToB64(asr.rawId),
        response: {
          clientDataJSON:    arrToB64(asr.response.clientDataJSON),
          authenticatorData: arrToB64(asr.response.authenticatorData),
          signature:         arrToB64(asr.response.signature),
          userHandle: asr.response.userHandle ? arrToB64(asr.response.userHandle) : null,
        }
      })
    });
    const vT = await vR.text();
    let res;
    try { res = JSON.parse(vT); } catch { throw new Error('Сервер: ' + vT.slice(0, 200)); }

    if (res.ok) {
      location.href = '/?page=admin';
    } else {
      throw new Error(res.error || 'Неверный ключ');
    }
  } catch(e) {
    err.textContent = e.message;
    err.classList.remove('hidden');
    btn.disabled = false;
  } finally {
    wait.classList.add('hidden');
  }
}

function backToPass() {
  document.getElementById('step-wa').classList.add('hidden');
  document.getElementById('step-pass').classList.remove('hidden');
  document.getElementById('pass-btn').disabled = false;
  document.getElementById('pass-btn').textContent = 'Далее →';
}
</script>
