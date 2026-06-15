<?php // profile.php
$u = current_user();
?>
<div class="page-head"><h1>⚙️ Профиль</h1></div>
<div class="profile-grid">
  <div class="card">
    <h2>👤 Информация</h2>
    <div class="info-row"><span>Имя</span><strong><?= htmlspecialchars($u['username']) ?></strong></div>
    <div class="info-row"><span>Email</span><strong><?= htmlspecialchars($u['email']) ?></strong></div>
    <div class="info-row"><span>Баланс</span><strong class="gold">💰 <?= number_format((float)$u['balance'],2,'.',' ') ?> монет</strong></div>
  </div>

  <div class="card">
    <h2>🔐 Двухфакторная аутентификация</h2>
    <?php if ($u['webauthn_enabled']): ?>
    <div class="twofa-status on"><span>✅</span><div><strong>2FA включена</strong><p>Аккаунт защищён WebAuthn ключом.</p></div></div>
    <button onclick="addKey()" class="btn btn-outline" style="margin-top:12px">➕ Добавить ещё ключ</button>
    <?php else: ?>
    <div class="twofa-status off"><span>⚠️</span><div><strong>2FA не настроена</strong><p>Защитите аккаунт через Touch ID, Face ID или USB-ключ.</p></div></div>
    <button onclick="addKey()" class="btn btn-primary" style="margin-top:12px" id="add-key-btn">🔑 Настроить 2FA (WebAuthn)</button>
    <?php endif; ?>
    <div id="wa-err"  class="err hidden" style="margin-top:10px"></div>
    <div id="wa-ok"   class="ok  hidden" style="margin-top:10px"></div>
    <div id="wa-wait" class="hidden" style="margin-top:10px;color:var(--t2);font-size:13px">⏳ Ожидаем устройство…</div>
  </div>
</div>

<script>
async function addKey() {
  const err  = document.getElementById('wa-err');
  const ok   = document.getElementById('wa-ok');
  const wait = document.getElementById('wa-wait');
  const btn  = document.getElementById('add-key-btn');
  err.classList.add('hidden'); ok.classList.add('hidden'); wait.classList.remove('hidden');
  if (btn) btn.disabled = true;

  if (!window.PublicKeyCredential) {
    err.textContent = 'Браузер не поддерживает WebAuthn';
    err.classList.remove('hidden'); wait.classList.add('hidden'); if(btn)btn.disabled=false; return;
  }

  try {
    const optR = await fetch('/?action=wa_reg_options', {method:'POST'});
    const optT = await optR.text();
    let opts;
    try { opts = JSON.parse(optT); } catch { throw new Error('Сервер: ' + optT.slice(0,200)); }
    if (opts.ok === false) throw new Error(opts.error);

    opts.challenge = b64ToArr(opts.challenge);
    opts.user.id   = b64ToArr(opts.user.id);
    if (opts.excludeCredentials) opts.excludeCredentials = opts.excludeCredentials.map(c=>({...c,id:b64ToArr(c.id)}));

    let cred;
    try { cred = await navigator.credentials.create({publicKey: opts}); }
    catch(e) { throw new Error(e.name==='NotAllowedError'?'Отменено или истекло время':e.name==='InvalidStateError'?'Ключ уже зарегистрирован':e.message); }

    const vR = await fetch('/?action=wa_reg_verify', {method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify({
      id: cred.id, type: cred.type, rawId: arrToB64(cred.rawId),
      response: {
        clientDataJSON:    arrToB64(cred.response.clientDataJSON),
        attestationObject: arrToB64(cred.response.attestationObject),
      }
    })});
    const vT = await vR.text();
    let res;
    try { res = JSON.parse(vT); } catch { throw new Error('Сервер: ' + vT.slice(0,200)); }
    if (res.ok) { ok.textContent = '✅ 2FA успешно настроена! Обновляем…'; ok.classList.remove('hidden'); setTimeout(()=>location.reload(),1500); }
    else throw new Error(res.error || 'Ошибка верификации');
  } catch(e) {
    err.textContent = e.message; err.classList.remove('hidden'); if(btn)btn.disabled=false;
  } finally { wait.classList.add('hidden'); }
}
</script>
