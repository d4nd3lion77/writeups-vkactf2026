<?php // home.php ?>
<div class="hero">
  <h1>🏅 Ставки на летние виды спорта</h1>
  <p>Виртуальные деньги — настоящий азарт. Победитель определяется через 5 минут!</p>
  <?php if (!$user): ?>
  <div style="margin-top:16px;display:flex;gap:10px;justify-content:center">
    <a href="/?page=register" class="btn btn-primary">Начать — 1000 монет бесплатно</a>
    <a href="/?page=login" class="btn btn-outline">Войти</a>
  </div>
  <?php endif; ?>
</div>

<div class="filter-bar">
  <button class="filter active" data-sport="all">🏅 Все</button>
  <button class="filter" data-sport="athletics">🏃 Лёгкая атлетика</button>
  <button class="filter" data-sport="swimming">🏊 Плавание</button>
  <button class="filter" data-sport="cycling">🚴 Велоспорт</button>
  <button class="filter" data-sport="weightlifting">🏋 Тяжёлая атлетика</button>
  <button class="filter" data-sport="boxing">🥊 Бокс</button>
  <button class="filter" data-sport="gymnastics">🤸 Гимнастика</button>
</div>

<div id="matches" class="matches-grid">
  <div class="loading">⏳ Загружаем матчи…</div>
</div>

<?php if ($user): ?>
<div id="bet-modal" class="modal hidden">
  <div class="overlay" onclick="closeModal()"></div>
  <div class="modal-box">
    <button class="modal-close" onclick="closeModal()">✕</button>
    <h2>Сделать ставку</h2>
    <div id="modal-info" class="modal-info"></div>
    <div class="quick-amounts">
      <button onclick="setAmt(50)">50</button>
      <button onclick="setAmt(100)">100</button>
      <button onclick="setAmt(250)">250</button>
      <button onclick="setAmt(500)">500</button>
    </div>
    <input type="number" id="bet-amount" class="input" placeholder="Сумма" min="10" max="10000" value="100" oninput="calcWin()">
    <div id="potential" class="potential"></div>
    <div id="bet-err" class="err hidden"></div>
    <button onclick="submitBet()" class="btn btn-primary" style="width:100%;margin-top:12px">Поставить</button>
    <input type="hidden" id="b-match"><input type="hidden" id="b-athlete"><input type="hidden" id="b-odds">
    <input type="hidden" id="b-csrf" value="<?= htmlspecialchars($csrf) ?>">
  </div>
</div>
<?php endif; ?>
