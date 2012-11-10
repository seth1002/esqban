create database marinas;
grant all privileges on marinas.* to 'sqba'@'%' with grant option;

CREATE TABLE `countries` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`name` varchar(50) NOT NULL,
	PRIMARY KEY (`id`) USING BTREE,
	UNIQUE (`name`),
	KEY `idx_name` (`name`) USING BTREE
) engine=innodb default charset=utf8;

CREATE TABLE `regions` (
    `id` INT NOT NULL AUTO_INCREMENT,
    `country` INT,
    `name` varchar(50) NOT NULL,
	PRIMARY KEY (`id`) USING BTREE,
	UNIQUE (`name`),
	KEY `idx_name` (`name`) USING BTREE
) engine=innodb default charset=utf8;

CREATE TABLE `marina_types` (
    `id` INT NOT NULL AUTO_INCREMENT,
    `name` varchar(50) NOT NULL,
	PRIMARY KEY (`id`) USING BTREE,
	UNIQUE (`name`),
	KEY `idx_name` (`name`) USING BTREE
) engine=innodb default charset=utf8;

CREATE TABLE `marinas` (
    `id` INT NOT NULL AUTO_INCREMENT,
    `name` varchar(50) NOT NULL,
    `region` INT,
    `latitude` float NOT NULL,
    `longitude` float NOT NULL,
    `type` INT,
	PRIMARY KEY (`id`) USING BTREE,
	UNIQUE (`name`),
	KEY `idx_name` (`name`) USING BTREE
) engine=innodb default charset=utf8;

CREATE OR REPLACE
VIEW vwMarinas
AS
SELECT m.name, m.latitude, m.longitude, t.name AS type, c.name AS country, r.name AS region
FROM marinas AS m
INNER JOIN marina_types t on t.id = m.type
INNER JOIN regions r on r.id = m.region
INNER JOIN countries c on c.id = r.country;
