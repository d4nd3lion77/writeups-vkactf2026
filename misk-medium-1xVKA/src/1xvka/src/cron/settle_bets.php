#!/usr/bin/env php
<?php
declare(strict_types=1);

function lg(string $m): void { echo '['.date('Y-m-d H:i:s').'] '.$m.PHP_EOL; }

$host = getenv('DB_HOST') ?: 'mysql';
$name = getenv('DB_NAME') ?: '1xvka';
$user = getenv('DB_USER') ?: '1xvka';
$pass = getenv('DB_PASS') ?: '';

// ── Wait for DB and tables ────────────────────────────────────────────────
$pdo   = null; $ready = false; $waited = 0;
while (!$ready && $waited < 120) {
    try {
        $pdo = new PDO("mysql:host={$host};dbname={$name};charset=utf8mb4", $user, $pass, [
            PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
            PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
        ]);
        $cnt = (int)$pdo->query("SELECT COUNT(*) FROM information_schema.tables WHERE table_schema='{$name}' AND table_name='matches'")->fetchColumn();
        if ($cnt > 0) { $ready = true; lg('DB and tables ready.'); }
        else { throw new RuntimeException('tables not yet created'); }
    } catch (Throwable $e) {
        $pdo = null; lg("Waiting ({$waited}s): ".$e->getMessage()); sleep(5); $waited += 5;
    }
}
if (!$ready) { lg('DB not ready after 120s. Exit.'); exit(1); }

// ── Generate initial matches if table is empty ────────────────────────────
$cnt = (int)$pdo->query('SELECT COUNT(*) FROM matches')->fetchColumn();
if ($cnt === 0) {
    lg('No matches found — generating initial set.');
    $sports = $pdo->query('SELECT id FROM sports')->fetchAll();
    foreach ($sports as $sp) {
        try {
            $st = $pdo->prepare('SELECT id,rank_score FROM athletes WHERE sport_id=? ORDER BY RAND() LIMIT 2');
            $st->execute([$sp['id']]);
            $ath = $st->fetchAll();
            if (count($ath) < 2) continue;
            [$a1,$a2] = $ath;
            $t = (float)$a1['rank_score']+(float)$a2['rank_score'];
            $o1 = max(1.10,min(9.99,round($t/(float)$a1['rank_score']*0.9,2)));
            $o2 = max(1.10,min(9.99,round($t/(float)$a2['rank_score']*0.9,2)));
            $pdo->prepare('INSERT INTO matches (sport_id,athlete1_id,athlete2_id,athlete1_odds,athlete2_odds,starts_at) VALUES (?,?,?,?,?,DATE_ADD(NOW(),INTERVAL 300 SECOND))')->execute([$sp['id'],$a1['id'],$a2['id'],$o1,$o2]);
            lg("  Created match: sport #{$sp['id']}  {$a1['id']}(x{$o1}) vs {$a2['id']}(x{$o2})");
        } catch(Throwable $e) { lg("  Error sport #{$sp['id']}: ".$e->getMessage()); }
    }
}

// ── Main loop ─────────────────────────────────────────────────────────────
lg('Cron loop started.');
while (true) {
    try {
        // 1. Settle expired matches
        $expired = $pdo->query('SELECT * FROM matches WHERE status="open" AND starts_at<=NOW()')->fetchAll();
        foreach ($expired as $m) {
            $pdo->beginTransaction();
            try {
                $s1 = $pdo->prepare('SELECT rank_score FROM athletes WHERE id=?'); $s1->execute([$m['athlete1_id']]); $sc1=(float)($s1->fetchColumn()?:1);
                $s2 = $pdo->prepare('SELECT rank_score FROM athletes WHERE id=?'); $s2->execute([$m['athlete2_id']]); $sc2=(float)($s2->fetchColumn()?:1);
                $winner = ((mt_rand()/mt_getrandmax())*($sc1+$sc2) <= $sc1) ? (int)$m['athlete1_id'] : (int)$m['athlete2_id'];
                $pdo->prepare('UPDATE matches SET status="settled",winner_id=?,settled_at=NOW() WHERE id=?')->execute([$winner,$m['id']]);

                $bst = $pdo->prepare('SELECT * FROM bets WHERE match_id=? AND status="pending"'); $bst->execute([$m['id']]);
                foreach ($bst->fetchAll() as $b) {
                    if ((int)$b['athlete_id']===$winner) {
                        $pay = round((float)$b['amount']*(float)$b['odds'],2);
                        $pdo->prepare('UPDATE bets SET status="won",payout=?,settled_at=NOW() WHERE id=?')->execute([$pay,$b['id']]);
                        $pdo->prepare('UPDATE users SET balance=balance+? WHERE id=?')->execute([$pay,$b['user_id']]);
                        lg("  Bet #{$b['id']}: WON +{$pay} → user #{$b['user_id']}");
                    } else {
                        $pdo->prepare('UPDATE bets SET status="lost",payout=0,settled_at=NOW() WHERE id=?')->execute([$b['id']]);
                        lg("  Bet #{$b['id']}: LOST → user #{$b['user_id']}");
                    }
                }
                $pdo->commit();
                lg("Match #{$m['id']} settled → winner #{$winner}");
            } catch(Throwable $e) { $pdo->rollBack(); lg("  Error match #{$m['id']}: ".$e->getMessage()); }
        }

        // 2. Generate new matches to keep board filled
        $sports = $pdo->query('SELECT id FROM sports')->fetchAll();
        foreach ($sports as $sp) {
            $cnt = $pdo->prepare('SELECT COUNT(*) FROM matches WHERE sport_id=? AND status="open"'); $cnt->execute([$sp['id']]);
            if ((int)$cnt->fetchColumn() >= 3) continue;
            $st = $pdo->prepare('SELECT id,rank_score FROM athletes WHERE sport_id=? AND id NOT IN (SELECT athlete1_id FROM matches WHERE sport_id=? AND status="open" UNION SELECT athlete2_id FROM matches WHERE sport_id=? AND status="open") ORDER BY RAND() LIMIT 2');
            $st->execute([$sp['id'],$sp['id'],$sp['id']]);
            $ath = $st->fetchAll();
            if (count($ath) < 2) { $st2=$pdo->prepare('SELECT id,rank_score FROM athletes WHERE sport_id=? ORDER BY RAND() LIMIT 2'); $st2->execute([$sp['id']]); $ath=$st2->fetchAll(); }
            if (count($ath) < 2) continue;
            [$a1,$a2]=$ath; $t=(float)$a1['rank_score']+(float)$a2['rank_score'];
            $o1=max(1.10,min(9.99,round($t/(float)$a1['rank_score']*0.9,2)));
            $o2=max(1.10,min(9.99,round($t/(float)$a2['rank_score']*0.9,2)));
            $pdo->prepare('INSERT INTO matches (sport_id,athlete1_id,athlete2_id,athlete1_odds,athlete2_odds,starts_at) VALUES (?,?,?,?,?,DATE_ADD(NOW(),INTERVAL 300 SECOND))')->execute([$sp['id'],$a1['id'],$a2['id'],$o1,$o2]);
            lg("  New match: sport #{$sp['id']} #{$a1['id']}(x{$o1}) vs #{$a2['id']}(x{$o2})");
        }
    } catch(Throwable $e) {
        lg('Loop error: '.$e->getMessage());
        try { $pdo = new PDO("mysql:host={$host};dbname={$name};charset=utf8mb4",$user,$pass,[PDO::ATTR_ERRMODE=>PDO::ERRMODE_EXCEPTION,PDO::ATTR_DEFAULT_FETCH_MODE=>PDO::FETCH_ASSOC]); } catch(Throwable) {}
    }
    sleep(60);
}
