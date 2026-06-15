<?php
declare(strict_types=1);

require_once __DIR__ . '/db.php';

const SPORT_ICONS = [
    'biathlon'      => '🎿',
    'cross-country' => '⛷',
    'speed-skating' => '⛸',
    'ice-hockey'    => '🏒',
];

function get_open_matches(): array {
    $rows = db()->query(
        'SELECT m.*,
                s.name AS sport_name, s.slug AS sport_slug,
                a1.name AS athlete1_name, a1.country AS athlete1_country, a1.country_code AS athlete1_cc,
                a2.name AS athlete2_name, a2.country AS athlete2_country, a2.country_code AS athlete2_cc
         FROM matches m
         JOIN sports s   ON s.id  = m.sport_id
         JOIN athletes a1 ON a1.id = m.athlete1_id
         JOIN athletes a2 ON a2.id = m.athlete2_id
         WHERE m.status = "open"
         ORDER BY m.starts_at ASC'
    )->fetchAll();

    foreach ($rows as &$r) {
        $r['sport_icon'] = SPORT_ICONS[$r['sport_slug']] ?? '🏅';
    }
    return $rows;
}

function get_bets_for_user(int $uid): array {
    $st = db()->prepare(
        'SELECT b.*, m.status AS match_status, m.winner_id,
                s.name AS sport_name, s.slug AS sport_slug,
                a.name  AS athlete_name,
                oa.name AS opponent_name
         FROM bets b
         JOIN matches m  ON m.id  = b.match_id
         JOIN sports s   ON s.id  = m.sport_id
         JOIN athletes a  ON a.id  = b.athlete_id
         JOIN athletes oa ON oa.id = IF(m.athlete1_id=b.athlete_id, m.athlete2_id, m.athlete1_id)
         WHERE b.user_id = ?
         ORDER BY b.placed_at DESC
         LIMIT 50'
    );
    $st->execute([$uid]);
    $rows = $st->fetchAll();
    foreach ($rows as &$r) $r['sport_icon'] = SPORT_ICONS[$r['sport_slug']] ?? '🏅';
    return $rows;
}

function place_bet(int $uid, int $matchId, int $athleteId, float $amount): array {
    if ($amount < 10)    return ['ok'=>false,'error'=>'Минимум 10 монет'];
    if ($amount > 10000) return ['ok'=>false,'error'=>'Максимум 10 000 монет'];

    $db = db();
    $st = $db->prepare('SELECT * FROM matches WHERE id=? AND status="open"');
    $st->execute([$matchId]);
    $match = $st->fetch();
    if (!$match) return ['ok'=>false,'error'=>'Матч не найден или закрыт'];

    $ids = [(int)$match['athlete1_id'], (int)$match['athlete2_id']];
    if (!in_array($athleteId, $ids, true)) return ['ok'=>false,'error'=>'Неверный участник'];

    $db->beginTransaction();
    try {
        $st = $db->prepare('SELECT balance FROM users WHERE id=? FOR UPDATE');
        $st->execute([$uid]);
        $bal = (float)$st->fetchColumn();
        if ($bal < $amount) { $db->rollBack(); return ['ok'=>false,'error'=>'Недостаточно средств']; }

        $ex = $db->prepare('SELECT id FROM bets WHERE user_id=? AND match_id=?');
        $ex->execute([$uid, $matchId]);
        if ($ex->fetch()) { $db->rollBack(); return ['ok'=>false,'error'=>'Ставка уже сделана']; }

        $odds = ($athleteId === (int)$match['athlete1_id']) ? $match['athlete1_odds'] : $match['athlete2_odds'];
        $db->prepare('UPDATE users SET balance=balance-? WHERE id=?')->execute([$amount, $uid]);
        $db->prepare('INSERT INTO bets (user_id,match_id,athlete_id,amount,odds) VALUES (?,?,?,?,?)')->execute([$uid,$matchId,$athleteId,$amount,$odds]);
        $db->commit();
        return ['ok'=>true,'odds'=>$odds,'potential'=>round($amount*$odds,2)];
    } catch (\Throwable $e) {
        $db->rollBack();
        return ['ok'=>false,'error'=>'Ошибка: '.$e->getMessage()];
    }
}

function generate_new_matches(): void {
    $db = db();
    $sports = $db->query('SELECT id FROM sports')->fetchAll();
    foreach ($sports as $sport) {
        try {
            $cnt = $db->prepare('SELECT COUNT(*) FROM matches WHERE sport_id=? AND status="open"');
            $cnt->execute([$sport['id']]);
            if ((int)$cnt->fetchColumn() >= 3) continue;

            $st = $db->prepare(
                'SELECT id,rank_score FROM athletes WHERE sport_id=?
                 AND id NOT IN (
                   SELECT athlete1_id FROM matches WHERE sport_id=? AND status="open"
                   UNION
                   SELECT athlete2_id FROM matches WHERE sport_id=? AND status="open"
                 ) ORDER BY RAND() LIMIT 2'
            );
            $st->execute([$sport['id'],$sport['id'],$sport['id']]);
            $ath = $st->fetchAll();

            if (count($ath) < 2) {
                $st = $db->prepare('SELECT id,rank_score FROM athletes WHERE sport_id=? ORDER BY RAND() LIMIT 2');
                $st->execute([$sport['id']]);
                $ath = $st->fetchAll();
            }
            if (count($ath) < 2) continue;

            [$a1,$a2] = $ath;
            $tot  = (float)$a1['rank_score'] + (float)$a2['rank_score'];
            $odd1 = max(1.10, min(9.99, round($tot/(float)$a1['rank_score']*0.9, 2)));
            $odd2 = max(1.10, min(9.99, round($tot/(float)$a2['rank_score']*0.9, 2)));
            $db->prepare(
                'INSERT INTO matches (sport_id,athlete1_id,athlete2_id,athlete1_odds,athlete2_odds,starts_at)
                 VALUES (?,?,?,?,?,DATE_ADD(NOW(),INTERVAL 300 SECOND))'
            )->execute([$sport['id'],$a1['id'],$a2['id'],$odd1,$odd2]);
        } catch (\Throwable) {}
    }
}
