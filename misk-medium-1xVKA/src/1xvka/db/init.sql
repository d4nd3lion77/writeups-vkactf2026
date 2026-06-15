SET NAMES utf8mb4;
SET time_zone = '+00:00';

CREATE TABLE IF NOT EXISTS users (
    id               INT AUTO_INCREMENT PRIMARY KEY,
    username         VARCHAR(64)   NOT NULL UNIQUE,
    email            VARCHAR(255)  NOT NULL UNIQUE,
    password_hash    VARCHAR(255)  NOT NULL,
    balance          DECIMAL(10,2) NOT NULL DEFAULT 1000.00,
    webauthn_enabled TINYINT(1)    NOT NULL DEFAULT 0,
    created_at       TIMESTAMP     DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS webauthn_credentials (
    id            INT AUTO_INCREMENT PRIMARY KEY,
    user_id       INT           NOT NULL,
    credential_id VARCHAR(512)  NOT NULL UNIQUE,
    public_key    MEDIUMTEXT    NOT NULL,
    sign_count    INT           NOT NULL DEFAULT 0,
    created_at    TIMESTAMP     DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS sports (
    id   INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    slug VARCHAR(50)  NOT NULL UNIQUE,
    icon VARCHAR(16)  NOT NULL DEFAULT ''
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS athletes (
    id           INT AUTO_INCREMENT PRIMARY KEY,
    sport_id     INT           NOT NULL,
    name         VARCHAR(150)  NOT NULL,
    country      VARCHAR(100)  NOT NULL,
    country_code CHAR(3)       NOT NULL,
    rank_score   DECIMAL(5,2)  NOT NULL DEFAULT 1.00,
    FOREIGN KEY (sport_id) REFERENCES sports(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS matches (
    id            INT AUTO_INCREMENT PRIMARY KEY,
    sport_id      INT          NOT NULL,
    athlete1_id   INT          NOT NULL,
    athlete2_id   INT          NOT NULL,
    athlete1_odds DECIMAL(6,2) NOT NULL,
    athlete2_odds DECIMAL(6,2) NOT NULL,
    status        ENUM('open','settled') NOT NULL DEFAULT 'open',
    winner_id     INT          DEFAULT NULL,
    starts_at     TIMESTAMP    NOT NULL,
    settled_at    TIMESTAMP    NULL DEFAULT NULL,
    FOREIGN KEY (sport_id)    REFERENCES sports(id),
    FOREIGN KEY (athlete1_id) REFERENCES athletes(id),
    FOREIGN KEY (athlete2_id) REFERENCES athletes(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS bets (
    id         INT AUTO_INCREMENT PRIMARY KEY,
    user_id    INT           NOT NULL,
    match_id   INT           NOT NULL,
    athlete_id INT           NOT NULL,
    amount     DECIMAL(10,2) NOT NULL,
    odds       DECIMAL(6,2)  NOT NULL,
    status     ENUM('pending','won','lost') NOT NULL DEFAULT 'pending',
    payout     DECIMAL(10,2) DEFAULT NULL,
    placed_at  TIMESTAMP     DEFAULT CURRENT_TIMESTAMP,
    settled_at TIMESTAMP     NULL DEFAULT NULL,
    FOREIGN KEY (user_id)    REFERENCES users(id),
    FOREIGN KEY (match_id)   REFERENCES matches(id),
    FOREIGN KEY (athlete_id) REFERENCES athletes(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ── Летние олимпийские виды спорта ───────────────────────────
INSERT INTO sports (id, name, slug, icon) VALUES
(1, 'Легкая атлетика',  'athletics',     '🏃'),
(2, 'Плавание',          'swimming',      '🏊'),
(3, 'Велоспорт',         'cycling',       '🚴'),
(4, 'Тяжелая атлетика', 'weightlifting', '🏋'),
(5, 'Бокс',              'boxing',        '🥊'),
(6, 'Гимнастика',        'gymnastics',    '🤸');

INSERT INTO athletes (sport_id, name, country, country_code, rank_score) VALUES
(1,'Ноа Лайлс','США','USA',2.40),
(1,'Марсель Жакобс','Италия','ITA',2.10),
(1,'Мондо Дюплантис','Швеция','SWE',2.35),
(1,'Фердинанд Омурья','Уганда','UGA',1.95),
(1,'Сифан Хасан','Нидерланды','NED',2.20),
(1,'Элейн Томпсон','Ямайка','JAM',2.15),
(1,'Жасмин Камачо','Пуэрто-Рико','PUR',1.90),
(1,'Атинг Му','США','USA',2.05),
(1,'Андерсон Питерс','Гренада','GRN',1.85),
(1,'Кевин Мэйер','Франция','FRA',1.80);

INSERT INTO athletes (sport_id, name, country, country_code, rank_score) VALUES
(2,'Леон Маршан','Франция','FRA',2.40),
(2,'Кэти Ледецки','США','USA',2.35),
(2,'Каелеб Дрессел','США','USA',2.25),
(2,'Арианна Титмус','Австралия','AUS',2.20),
(2,'Адам Пити','Великобритания','GBR',2.15),
(2,'Сара Шестром','Швеция','SWE',2.10),
(2,'Дэвид Поповичи','Румыния','ROU',2.05),
(2,'Чжан Юйфэй','Китай','CHN',2.00),
(2,'Маргарет МакНил','Канада','CAN',1.95),
(2,'Тацухиро Тамару','Япония','JPN',1.80);

INSERT INTO athletes (sport_id, name, country, country_code, rank_score) VALUES
(3,'Тадей Погачар','Словения','SLO',2.50),
(3,'Ремко Эвенпул','Бельгия','BEL',2.30),
(3,'Йонас Вингегорд','Дания','DEN',2.20),
(3,'Матье ван дер Пул','Нидерланды','NED',2.10),
(3,'Том Пидкок','Великобритания','GBR',2.00),
(3,'Примож Роглич','Словения','SLO',2.15),
(3,'Марианна Вос','Нидерланды','NED',2.05),
(3,'Элиза Лонго Борини','Италия','ITA',1.85),
(3,'Ван Аерт Вут','Бельгия','BEL',1.90),
(3,'Криста Пенни','Канада','CAN',1.75);

INSERT INTO athletes (sport_id, name, country, country_code, rank_score) VALUES
(4,'Ли Фабинь','Китай','CHN',2.30),
(4,'Лю Хуаньхуа','Китай','CHN',2.20),
(4,'Рижа Заиру','Грузия','GEO',2.05),
(4,'Кристиан Иба','Колумбия','COL',1.95),
(4,'Ло Тан','Тайвань','TPE',2.10),
(4,'Со Ын-А','Южная Корея','KOR',2.00),
(4,'Хиди Туманоу','Самоа','SAM',1.90),
(4,'Антон Плесников','Израиль','ISR',1.85),
(4,'Милаима Асипели','Тонга','TGA',1.75),
(4,'Кристиан Сонг','Индонезия','INA',1.80);

INSERT INTO athletes (sport_id, name, country, country_code, rank_score) VALUES
(5,'Имане Хелиф','Алжир','ALG',2.30),
(5,'Лазаро Альварес','Куба','CUB',2.20),
(5,'Ловлин Борогейн','Индия','IND',2.00),
(5,'Цяо Синьюй','Китай','CHN',2.10),
(5,'Джесси Вальдес','США','USA',2.05),
(5,'Дуку Малика','Гвинея','GUI',1.95),
(5,'Хулиан Луна','Колумбия','COL',1.90),
(5,'Марко Ла Дженезе','Италия','ITA',1.85),
(5,'Абельт Анкош','Нидерланды','NED',1.80),
(5,'Юй Усман','Китай','CHN',2.15);

INSERT INTO athletes (sport_id, name, country, country_code, rank_score) VALUES
(6,'Симона Байлс','США','USA',2.60),
(6,'Карлос Юло','Филиппины','PHI',2.30),
(6,'Риото Миязаки','Япония','JPN',2.25),
(6,'Чжоу Гуань','Китай','CHN',2.20),
(6,'Ребека Андраде','Бразилия','BRA',2.15),
(6,'Алия Мустафина','Казахстан','KAZ',1.95),
(6,'Нина Дервал','Бельгия','BEL',1.90),
(6,'Карли Паттерсон','США','USA',2.00),
(6,'Арто Пало','Финляндия','FIN',1.85),
(6,'Дилназа Жексибекова','Казахстан','KAZ',1.80);

INSERT INTO matches (sport_id, athlete1_id, athlete2_id, athlete1_odds, athlete2_odds, starts_at) VALUES
(1, 1,  2,  1.78, 2.12, DATE_ADD(NOW(), INTERVAL 300 SECOND)),
(2, 11, 12, 1.49, 1.79, DATE_ADD(NOW(), INTERVAL 300 SECOND)),
(3, 21, 22, 1.44, 1.68, DATE_ADD(NOW(), INTERVAL 300 SECOND)),
(4, 31, 32, 1.65, 1.80, DATE_ADD(NOW(), INTERVAL 300 SECOND)),
(5, 41, 50, 1.63, 1.76, DATE_ADD(NOW(), INTERVAL 300 SECOND)),
(6, 51, 52, 1.30, 1.62, DATE_ADD(NOW(), INTERVAL 300 SECOND));
