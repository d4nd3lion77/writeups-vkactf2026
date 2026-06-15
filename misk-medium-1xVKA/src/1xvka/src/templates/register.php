<?php // register.php ?>
<div class="auth-page">
<div class="auth-card">
  <div class="auth-head"><div class="auth-icon">🏅</div><h1>Регистрация</h1><p>Получите 1000 монет бесплатно</p></div>
  <form onsubmit="doRegister(event)">
    <input type="hidden" id="reg-csrf" value="<?= htmlspecialchars($csrf) ?>">
    <div class="field"><label>Имя пользователя</label><input id="reg-u" class="input" type="text" placeholder="olympian_2026" required minlength="3" maxlength="32" autocomplete="username"></div>
    <div class="field"><label>Email</label><input id="reg-e" class="input" type="email" placeholder="you@email.com" required autocomplete="email"></div>
    <div class="field"><label>Пароль</label><input id="reg-p" class="input" type="password" placeholder="Минимум 8 символов" required minlength="8" autocomplete="new-password"></div>
    <div id="reg-err" class="err hidden"></div>
    <button id="reg-btn" class="btn btn-primary" style="width:100%">Зарегистрироваться</button>
  </form>
  <p class="auth-foot">Уже есть аккаунт? <a href="/?page=login">Войти</a></p>
</div>
</div>

<script>
async function doRegister(e) {
  e.preventDefault();
  const btn = document.getElementById('reg-btn');
  const err = document.getElementById('reg-err');
  btn.disabled = true; btn.textContent = 'Регистрируем…'; err.classList.add('hidden');
  try {
    const r = await fetch('/?action=register', {method:'POST', body: new URLSearchParams({
      csrf: document.getElementById('reg-csrf').value,
      username: document.getElementById('reg-u').value.trim(),
      email: document.getElementById('reg-e').value.trim(),
      password: document.getElementById('reg-p').value,
    })});
    const d = await r.json();
    if (d.ok) { location.href = '/'; }
    else { err.textContent = d.error; err.classList.remove('hidden'); btn.disabled=false; btn.textContent='Зарегистрироваться'; }
  } catch(ex) { err.textContent='Ошибка сети'; err.classList.remove('hidden'); btn.disabled=false; btn.textContent='Зарегистрироваться'; }
}
</script>
