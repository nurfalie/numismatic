CREATE DATABASE xnumismatic_db; USE xnumismatic_db;
CREATE USER xnumismatic_user IDENTIFIED BY 'xnumismatic_user';

CREATE TABLE coin
(
	name		VARCHAR(64) NOT NULL,
	country		VARCHAR(32) NOT NULL,
	grade		VARCHAR(32) NOT NULL,
	description	LONGTEXT NULL,
	composition	VARCHAR(32) NOT NULL,
	weight		VARCHAR(32) NOT NULL,
	year		VARCHAR(4) NOT NULL DEFAULT 'N/A',
	face_val	FLOAT(10,2) NOT NULL DEFAULT 0.00,
	acq_val		FLOAT(10,2) NOT NULL DEFAULT 0.00,
	market_val	FLOAT(10,2) NOT NULL DEFAULT 0.00,
	quantity	INT(10) NOT NULL DEFAULT 1,
	user_name	VARCHAR(32) NOT NULL PRIMARY KEY,
	id		INT(10) NOT NULL UNIQUE KEY DEFAULT 1
);

GRANT ALL ON xnumismatic_db.* TO xnumismatic_user@localhost IDENTIFIED BY 'xnumismatic_user';
