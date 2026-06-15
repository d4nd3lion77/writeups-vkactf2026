'use strict';

// ─── Base64url utils ──────────────────────────────────────────────────────
function b64ToArr(b64) {
    const s = atob(b64.replace(/-/g,'+').replace(/_/g,'/').padEnd(b64.length + (4 - b64.length % 4) % 4, '='));
    return Uint8Array.from(s, c => c.charCodeAt(0)).buffer;
}
function arrToB64(buf) {
    const bytes = new Uint8Array(buf instanceof ArrayBuffer ? buf : buf.buffer || buf);
    let s = ''; bytes.forEach(b => s += String.fromCharCode(b));
    return btoa(s).replace(/\+/g,'-').replace(/\//g,'_').replace(/=/g,'');
}

// ─── Toast ────────────────────────────────────────────────────────────────
function showToast(msg, type='success') {
    const t = document.getElementById('toast'); if (!t) return;
    t.textContent = msg; t.className = 'toast ' + type;
    t.classList.remove('hidden');
    clearTimeout(t._t); t._t = setTimeout(() => t.classList.add('hidden'), 3000);
}

// ─── Flags ────────────────────────────────────────────────────────────────
function flag(cc) {
    const m={NOR:'🇳🇴',FRA:'🇫🇷',SWE:'🇸🇪',GER:'🇩🇪',AUT:'🇦🇹',ITA:'🇮🇹',RUS:'🇷🇺',FIN:'🇫🇮',NED:'🇳🇱',CAN:'🇨🇦',USA:'🇺🇸',CZE:'🇨🇿',SUI:'🇨🇭',SVK:'🇸🇰',JAM:'🇯🇲',AUS:'🇦🇺',GBR:'🇬🇧',CHN:'🇨🇳',JPN:'🇯🇵',BRA:'🇧🇷',UGA:'🇺🇬',PUR:'🇵🇷',GRN:'🇬🇩',ROU:'🇷🇴',SLO:'🇸🇮',DEN:'🇩🇰',BEL:'🇧🇪',GEO:'🇬🇪',COL:'🇨🇴',TPE:'🇹🇼',KOR:'🇰🇷',SAM:'🇼🇸',ISR:'🇮🇱',TGA:'🇹🇴',INA:'🇮🇩',ALG:'🇩🇿',GUI:'🇬🇳',CUB:'🇨🇺',IND:'🇮🇳',PHI:'🇵🇭',KAZ:'🇰🇿'};
    return m[cc] || '🏳️';
}

function esc(s) {
    return String(s).replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;').replace(/"/g,'&quot;');
}

function fmtSecs(s) {
    if (s <= 0) return '⏳ Идёт…';
    return Math.floor(s/60) + ':' + String(s%60).padStart(2,'0');
}

function fmtDate(ds) {
    return new Date(ds.replace(' ','T')+'Z').toLocaleString('ru-RU',{day:'2-digit',month:'2-digit',hour:'2-digit',minute:'2-digit'});
}

// ─── Logout ───────────────────────────────────────────────────────────────
async function doLogout() {
    await fetch('/?action=logout', {method:'POST'});
    location.href = '/';
}

// ─── Matches ──────────────────────────────────────────────────────────────
let allMatches = [], activeFilter = 'all', cTimer = null;

async function loadMatches() {
    const g = document.getElementById('matches'); if (!g) return;
    try {
        const r = await fetch('/?action=get_matches', {method:'POST'});
        if (!r.ok) { g.innerHTML = '<div class="no-m">❌ Ошибка сервера '+r.status+'</div>'; return; }
        const d = await r.json();
        if (!Array.isArray(d)) { g.innerHTML = '<div class="no-m">❌ '+(d.error||'Неверный ответ')+'</div>'; return; }
        allMatches = d; renderMatches();
    } catch(e) { g.innerHTML = '<div class="no-m">❌ ' + e.message + '</div>'; }
}

function renderMatches() {
    const g = document.getElementById('matches'); if (!g) return;
    const list = activeFilter === 'all' ? allMatches : allMatches.filter(m => m.sport_slug === activeFilter);
    if (!list.length) { g.innerHTML = '<div class="no-m">🏔️ Матчей пока нет — скоро появятся новые!</div>'; return; }

    const loggedIn = !!document.getElementById('bal');

    g.innerHTML = list.map(m => {
        const secs = Math.max(0, Math.floor((new Date(m.starts_at.replace(' ','T')+'Z') - Date.now()) / 1000));
        return `<div class="match-card" data-slug="${m.sport_slug}">
  <div class="mc-head"><span>${m.sport_icon} ${esc(m.sport_name)}</span><span class="countdown" data-ts="${m.starts_at}">${fmtSecs(secs)}</span></div>
  <div class="mc-body">
    <div class="athlete"><span class="aflag">${flag(m.athlete1_cc)}</span><div class="aname">${esc(m.athlete1_name)}</div><div class="acountry">${esc(m.athlete1_country)}</div><div class="odds">x${m.athlete1_odds}</div></div>
    <div class="vs">VS</div>
    <div class="athlete"><span class="aflag">${flag(m.athlete2_cc)}</span><div class="aname">${esc(m.athlete2_name)}</div><div class="acountry">${esc(m.athlete2_country)}</div><div class="odds">x${m.athlete2_odds}</div></div>
  </div>
  ${loggedIn ? `
  <div class="mc-foot">
    <button class="btn btn-outline btn-sm" onclick="openModal(${m.id},${m.athlete1_id},'${esc(m.athlete1_name)}',${m.athlete1_odds},'${m.sport_icon} ${esc(m.sport_name)}')">Ставка на ${flag(m.athlete1_cc)}</button>
    <button class="btn btn-outline btn-sm" onclick="openModal(${m.id},${m.athlete2_id},'${esc(m.athlete2_name)}',${m.athlete2_odds},'${m.sport_icon} ${esc(m.sport_name)}')">Ставка на ${flag(m.athlete2_cc)}</button>
  </div>` : `
  <div class="mc-foot"><a href="/?page=login" class="btn btn-primary btn-sm" style="width:100%">🔐 Войдите чтобы ставить</a></div>`}
</div>`;
    }).join('');

    startCountdowns();
}

function startCountdowns() {
    if (cTimer) clearInterval(cTimer);
    cTimer = setInterval(() => {
        document.querySelectorAll('.countdown[data-ts]').forEach(el => {
            const s = Math.max(0, Math.floor((new Date(el.dataset.ts.replace(' ','T')+'Z') - Date.now()) / 1000));
            el.textContent = fmtSecs(s);
            if (s === 0) el.closest('.match-card').style.opacity = '0.5';
        });
    }, 1000);
}

// ─── Filters ──────────────────────────────────────────────────────────────
function initFilters() {
    document.querySelectorAll('.filter').forEach(b => b.addEventListener('click', () => {
        document.querySelectorAll('.filter').forEach(x => x.classList.remove('active'));
        b.classList.add('active'); activeFilter = b.dataset.sport; renderMatches();
    }));
}

// ─── Bet modal ────────────────────────────────────────────────────────────
let curOdds = 1;

function openModal(matchId, athleteId, name, odds, sport) {
    document.getElementById('b-match').value   = matchId;
    document.getElementById('b-athlete').value = athleteId;
    document.getElementById('b-odds').value    = odds;
    curOdds = parseFloat(odds);
    document.getElementById('modal-info').innerHTML = `<strong>${sport}</strong><br>Ставка на: <strong>${esc(name)}</strong><br>Коэффициент: <strong class="gold">x${odds}</strong>`;
    document.getElementById('bet-amount').value = 100;
    document.getElementById('bet-err').classList.add('hidden');
    calcWin();
    document.getElementById('bet-modal').classList.remove('hidden');
    setTimeout(() => document.getElementById('bet-amount').focus(), 80);
}

function closeModal() { document.getElementById('bet-modal').classList.add('hidden'); }
function setAmt(v) { document.getElementById('bet-amount').value = v; calcWin(); }
function calcWin() {
    const a = parseFloat(document.getElementById('bet-amount').value) || 0;
    const el = document.getElementById('potential');
    el.textContent = a > 0 ? `💰 Возможный выигрыш: ${(a * curOdds).toFixed(2)} монет` : '';
}

async function submitBet() {
    const amount = parseFloat(document.getElementById('bet-amount').value);
    const err = document.getElementById('bet-err');
    err.classList.add('hidden');
    if (!amount || amount < 10) { err.textContent = 'Минимум 10 монет'; err.classList.remove('hidden'); return; }

    try {
        const r = await fetch('/?action=place_bet', {method:'POST', body: new URLSearchParams({
            match_id:   document.getElementById('b-match').value,
            athlete_id: document.getElementById('b-athlete').value,
            amount,
            csrf:       document.getElementById('b-csrf').value,
        })});
        const d = await r.json();
        if (d.ok) {
            closeModal();
            showToast(`✅ Ставка принята! Возможный выигрыш: ${d.potential} монет`, 'success');
            refreshBal(); setTimeout(loadMatches, 500);
        } else { err.textContent = d.error; err.classList.remove('hidden'); }
    } catch(e) { err.textContent = 'Ошибка сети'; err.classList.remove('hidden'); }
}

// ─── Balance ──────────────────────────────────────────────────────────────
async function refreshBal() {
    try {
        const r = await fetch('/?action=get_balance', {method:'POST'});
        const d = await r.json();
        const el = document.getElementById('bal');
        if (el && d.balance) { el.textContent = parseFloat(d.balance).toLocaleString('ru-RU',{minimumFractionDigits:2}); el.style.color='var(--green)'; setTimeout(()=>el.style.color='',600); }
    } catch {}
}

// ─── My Bets ──────────────────────────────────────────────────────────────
async function loadBets() {
    const list = document.getElementById('bets-list'); if (!list) return;
    try {
        const r = await fetch('/?action=get_bets', {method:'POST'});
        const bets = await r.json();
        if (!Array.isArray(bets)) { list.innerHTML = '<div class="no-m">❌ '+(bets.error||'Ошибка')+'</div>'; return; }

        const sv = (id,v) => { const e=document.getElementById(id); if(e) e.textContent=v; };
        sv('s-total', bets.length);
        sv('s-won',   bets.filter(b=>b.status==='won').length);
        sv('s-lost',  bets.filter(b=>b.status==='lost').length);
        sv('s-pend',  bets.filter(b=>b.status==='pending').length);

        if (!bets.length) { list.innerHTML = '<div class="no-m">📋 Ставок пока нет. <a href="/">Перейти к матчам →</a></div>'; return; }

        const sl={'won':'✅ Выиграл','lost':'❌ Проиграл','pending':'⏳ Ожидает'};
        const sc={'won':'g','lost':'r','pending':'y'};
        list.innerHTML = bets.map(b => {
            const icon = {'biathlon':'🎿','cross-country':'⛷','speed-skating':'⛸','ice-hockey':'🏒'}[b.sport_slug]||'🏅';
            let pay = '';
            if (b.status==='won')     pay = `<span class="green">+${parseFloat(b.payout).toFixed(2)}</span>`;
            else if (b.status==='lost') pay = `<span class="red">−${parseFloat(b.amount).toFixed(2)}</span>`;
            else pay = `<span class="gold">${(parseFloat(b.amount)*parseFloat(b.odds)).toFixed(2)}*</span>`;
            return `<div class="bet-row">
  <span class="bet-icon">${icon}</span>
  <div class="bet-info"><div class="bet-match">${esc(b.athlete_name)} vs ${esc(b.opponent_name)}</div><div class="bet-meta">${esc(b.sport_name)} · ${fmtDate(b.placed_at)}</div><div class="bet-meta">${pay}</div></div>
  <div class="bet-right"><div class="bet-amt">${parseFloat(b.amount).toFixed(2)} 🪙</div><div class="bet-odds">x${b.odds}</div></div>
  <span class="badge ${sc[b.status]||'gr'}">${sl[b.status]||b.status}</span>
</div>`;
        }).join('');
    } catch(e) { list.innerHTML = '<div class="no-m">❌ ' + e.message + '</div>'; }
}

// ─── Keyboard ─────────────────────────────────────────────────────────────
document.addEventListener('keydown', e => { if (e.key==='Escape') closeModal(); });

// ─── Init ─────────────────────────────────────────────────────────────────
document.addEventListener('DOMContentLoaded', () => {
    if (document.getElementById('matches')) {
        initFilters(); loadMatches();
        setInterval(loadMatches, 30000);
        if (document.getElementById('bal')) setInterval(refreshBal, 15000);
    }
    if (document.getElementById('bets-list')) {
        loadBets(); setInterval(loadBets, 10000);
        if (document.getElementById('bal')) setInterval(refreshBal, 15000);
    }
});
