<?php // admin.php
$db = db();
$stats = [
    'users'   => (int)$db->query('SELECT COUNT(*) FROM users')->fetchColumn(),
    'bets'    => (int)$db->query('SELECT COUNT(*) FROM bets')->fetchColumn(),
    'pending' => (int)$db->query('SELECT COUNT(*) FROM bets WHERE status="pending"')->fetchColumn(),
    'open'    => (int)$db->query('SELECT COUNT(*) FROM matches WHERE status="open"')->fetchColumn(),
    'wagered' => (float)$db->query('SELECT COALESCE(SUM(amount),0) FROM bets')->fetchColumn(),
    'paid'    => (float)$db->query('SELECT COALESCE(SUM(payout),0) FROM bets WHERE status="won"')->fetchColumn(),
];
$users = $db->query(
    'SELECT u.*,COUNT(b.id) bc FROM users u LEFT JOIN bets b ON b.user_id=u.id GROUP BY u.id ORDER BY u.created_at DESC LIMIT 100'
)->fetchAll();
$bets = $db->query(
    'SELECT b.*,u.username,a.name an,s.slug sport_slug FROM bets b
     JOIN users u ON u.id=b.user_id JOIN athletes a ON a.id=b.athlete_id
     JOIN matches m ON m.id=b.match_id JOIN sports s ON s.id=m.sport_id
     ORDER BY b.placed_at DESC LIMIT 100'
)->fetchAll();
$matches = $db->query(
    'SELECT m.*,s.name sn,s.slug slug,a1.name a1n,a1.country a1c,a2.name a2n,a2.country a2c,
            COUNT(b.id) bc,COALESCE(SUM(b.amount),0) tw
     FROM matches m JOIN sports s ON s.id=m.sport_id
     JOIN athletes a1 ON a1.id=m.athlete1_id JOIN athletes a2 ON a2.id=m.athlete2_id
     LEFT JOIN bets b ON b.match_id=m.id WHERE m.status="open" GROUP BY m.id ORDER BY m.starts_at ASC LIMIT 50'
)->fetchAll();
$icons = ['biathlon'=>'🎿','cross-country'=>'⛷','speed-skating'=>'⛸','ice-hockey'=>'🏒'];

// Secret note from .env
env_load();
$noteRaw = getenv('ADMIN_NOTE') ?: '(заметка не задана — добавьте ADMIN_NOTE= в .env)';
// Allow literal \n in .env value to become real newlines
$note = str_replace('\\n', "\n", $noteRaw);
?>
<meta name="csrf" content="<?= htmlspecialchars($csrf) ?>">

<div class="admin-head">
  <div><h1>🛡️ Панель администратора</h1><p class="sub">1xVKA</p></div>
  <button onclick="aLogout()" class="btn btn-danger btn-sm">Выйти из панели</button>
</div>

<div class="adm-stats">
  <div class="adm-stat"><div class="sv"><?= $stats['users'] ?></div><div class="sl">👤 Пользователей</div></div>
  <div class="adm-stat"><div class="sv"><?= $stats['bets'] ?></div><div class="sl">🎰 Ставок</div></div>
  <div class="adm-stat warn"><div class="sv"><?= $stats['pending'] ?></div><div class="sl">⏳ Ожидают</div></div>
  <div class="adm-stat"><div class="sv"><?= $stats['open'] ?></div><div class="sl">⚡ Матчей открыто</div></div>
  <div class="adm-stat blue"><div class="sv"><?= number_format($stats['wagered'],0,'.',' ') ?></div><div class="sl">💰 Поставлено</div></div>
  <div class="adm-stat green"><div class="sv"><?= number_format($stats['paid'],0,'.',' ') ?></div><div class="sl">🏆 Выплачено</div></div>
</div>

<div class="adm-tabs">
  <button class="atab active" onclick="showTab('tu',this)">👤 Пользователи (<?= count($users) ?>)</button>
  <button class="atab" onclick="showTab('tb',this)">🎰 Ставки (<?= count($bets) ?>)</button>
  <button class="atab" onclick="showTab('tm',this)">⚡ Матчи (<?= count($matches) ?>)</button>
  <button class="atab" onclick="showTab('tn',this)">📝 Заметка</button>
</div>

<!-- Users -->
<div id="tu" class="atab-c">
<div class="tbl-wrap"><table class="tbl">
<thead><tr><th>#</th><th>Имя</th><th>Email</th><th>Баланс</th><th>Ставок</th><th>Дата</th><th>Действия</th></tr></thead>
<tbody>
<?php foreach ($users as $u): ?>
<tr>
  <td class="dim"><?= $u['id'] ?></td>
  <td><strong><?= htmlspecialchars($u['username']) ?></strong></td>
  <td class="dim"><?= htmlspecialchars($u['email']) ?></td>
  <td class="gold">💰 <?= number_format((float)$u['balance'],2,'.',' ') ?></td>
  <td class="ctr"><?= $u['bc'] ?></td>
  <td class="dim"><?= date('d.m.Y H:i', strtotime($u['created_at'])) ?></td>
  <td>
    <button class="tbtn" onclick="adjBal(<?= $u['id'] ?>,'<?= htmlspecialchars($u['username'],ENT_QUOTES) ?>')">± Баланс</button>
  </td>
</tr>
<?php endforeach; ?>
</tbody></table></div>
</div>

<!-- Bets -->
<div id="tb" class="atab-c hidden">
<div class="tbl-wrap"><table class="tbl">
<thead><tr><th>#</th><th>Спорт</th><th>Игрок</th><th>Участник</th><th>Сумма</th><th>Коэф</th><th>Статус</th><th>Выплата</th><th>Время</th></tr></thead>
<tbody>
<?php foreach ($bets as $b):
  $sc=['won'=>'g','lost'=>'r','pending'=>'y'][$b['status']]??'gr';
  $sl=['won'=>'✅ Выиграл','lost'=>'❌ Проиграл','pending'=>'⏳ Ожидает'][$b['status']]??$b['status'];
  $ic=$icons[$b['sport_slug']]??'🏅';
?>
<tr>
  <td class="dim"><?= $b['id'] ?></td>
  <td><?= $ic ?></td>
  <td><strong><?= htmlspecialchars($b['username']) ?></strong></td>
  <td><?= htmlspecialchars($b['an']) ?></td>
  <td class="gold"><?= number_format((float)$b['amount'],2) ?></td>
  <td>x<?= $b['odds'] ?></td>
  <td><span class="badge <?= $sc ?>"><?= $sl ?></span></td>
  <td class="gold"><?= $b['status']==='won'?'+'.number_format((float)$b['payout'],2):($b['status']==='lost'?'—':number_format((float)$b['amount']*(float)$b['odds'],2).'*') ?></td>
  <td class="dim"><?= date('d.m H:i',strtotime($b['placed_at'])) ?></td>
</tr>
<?php endforeach; ?>
</tbody></table></div>
</div>

<!-- Matches -->
<div id="tm" class="atab-c hidden">
<div style="margin-bottom:10px">
  <button class="btn btn-primary btn-sm" onclick="genMatches()">⚡ Сгенерировать матчи</button>
</div>
<div class="tbl-wrap"><table class="tbl">
<thead><tr><th>#</th><th>Спорт</th><th>Участник 1</th><th>К1</th><th>Участник 2</th><th>К2</th><th>Ставок</th><th>Поставлено</th><th>Завершение</th></tr></thead>
<tbody>
<?php foreach ($matches as $m): $ic = $icons[$m['slug']]??'🏅'; ?>
<tr>
  <td class="dim"><?= $m['id'] ?></td>
  <td><?= $ic ?> <?= htmlspecialchars($m['sn']) ?></td>
  <td><?= htmlspecialchars($m['a1n']) ?><br><small class="dim"><?= $m['a1c'] ?></small></td>
  <td class="purp">x<?= $m['athlete1_odds'] ?></td>
  <td><?= htmlspecialchars($m['a2n']) ?><br><small class="dim"><?= $m['a2c'] ?></small></td>
  <td class="purp">x<?= $m['athlete2_odds'] ?></td>
  <td class="ctr"><?= $m['bc'] ?></td>
  <td class="gold"><?= number_format((float)$m['tw'],0) ?></td>
  <td class="dim"><?= date('H:i:s',strtotime($m['starts_at'])) ?></td>
</tr>
<?php endforeach; ?>
</tbody></table></div>
</div>

<!-- Secret Note -->
<div id="tn" class="atab-c hidden">
  <div class="note-card">
    <div class="note-head">📝 Секретная заметка <span class="note-src">из .env → ADMIN_NOTE</span></div>
    <div class="note-body"><?= nl2br(htmlspecialchars($note)) ?></div>
  </div>
</div>

<!-- Balance modal -->
<div id="bal-modal" class="modal hidden">
  <div class="overlay" onclick="document.getElementById('bal-modal').classList.add('hidden')"></div>
  <div class="modal-box">
    <button class="modal-close" onclick="document.getElementById('bal-modal').classList.add('hidden')">✕</button>
    <h2>Изменить баланс</h2>
    <p id="bal-who" style="color:var(--t2);margin-bottom:12px;font-size:14px"></p>
    <div class="field"><label>Сумма (+начислить / −списать)</label><input id="bal-amt" class="input" type="number" placeholder="500 или -200"></div>
    <div id="bal-err" class="err hidden"></div>
    <button onclick="submitBal()" class="btn btn-primary" style="width:100%">Применить</button>
    <input type="hidden" id="bal-uid">
  </div>
</div>

<script>
function showTab(id,btn){
  document.querySelectorAll('.atab-c').forEach(t=>t.classList.add('hidden'));
  document.querySelectorAll('.atab').forEach(t=>t.classList.remove('active'));
  document.getElementById(id).classList.remove('hidden');
  btn.classList.add('active');
}
async function aLogout(){await fetch('/?action=admin_logout',{method:'POST'});location.href='/';}
async function genMatches(){
  const r=await fetch('/?action=get_matches',{method:'POST'});
  const d=await r.json();
  showToast('✅ '+(Array.isArray(d)?d.length+' матчей':'Готово'),'success');
  setTimeout(()=>location.reload(),1200);
}
function adjBal(uid,name){
  document.getElementById('bal-uid').value=uid;
  document.getElementById('bal-who').textContent='Пользователь: '+name;
  document.getElementById('bal-amt').value='';
  document.getElementById('bal-err').classList.add('hidden');
  document.getElementById('bal-modal').classList.remove('hidden');
  setTimeout(()=>document.getElementById('bal-amt').focus(),100);
}
async function submitBal(){
  const uid=document.getElementById('bal-uid').value;
  const amt=parseFloat(document.getElementById('bal-amt').value);
  const err=document.getElementById('bal-err');
  if(!amt){err.textContent='Введите сумму';err.classList.remove('hidden');return;}
  const csrf=document.querySelector('meta[name=csrf]').content;
  const r=await fetch('/?action=admin_adjust_balance',{method:'POST',body:new URLSearchParams({user_id:uid,amount:amt,csrf})});
  const d=await r.json();
  if(d.ok){document.getElementById('bal-modal').classList.add('hidden');showToast('✅ Баланс изменён','success');setTimeout(()=>location.reload(),1000);}
  else{err.textContent=d.error;err.classList.remove('hidden');}
}
</script>
