
CREATE TABLE `sessions` (
  `date_` datetime NOT NULL,
  `sessionid` int(11) NOT NULL COMMENT 'unix timestamp',
  PRIMARY KEY (`sessionid`,`date_`) USING BTREE,
  KEY `idx_sessionid` (`sessionid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `sectors` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `name_UNIQUE` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
  
CREATE TABLE `companies` (
  `sessionid` int(11) NOT NULL COMMENT 'unix timestamp',
  `category` varchar(8) NOT NULL,
  `symbol` varchar(8) NOT NULL,
  `name` varchar(50) NOT NULL,
  `sector` int(11) DEFAULT NULL,
  `subsector` int(11) DEFAULT NULL,
  `url` text,
  `tase_url` text,
  PRIMARY KEY (`symbol`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `financial_statements` (
  `sessionid` int(11) not null comment "unix timestamp",
  `symbol` varchar(8) not null,
  `period` varchar(20) NOT NULL,
  `total_assets` bigint(20) NOT NULL,
  `current_assets` bigint(20) NOT NULL,
  `non_current_assets` bigint(20) NOT NULL,
  `shareholders_equity` bigint(20) NOT NULL,
  `of_which_minority_interest` bigint(20) NOT NULL,
  `current_liabilities` bigint(20) NOT NULL,
  `long_term_liabilities` bigint(20) NOT NULL,
  `revenues` bigint(20) NOT NULL,
  `gross_profit` bigint(20) NOT NULL,
  `operating_income` bigint(20) NOT NULL,
  `income_before_tax` bigint(20) NOT NULL,
  `net_income` bigint(20) NOT NULL,
  `income_atributable_to_shareholders` bigint(20) NOT NULL,
  `earnings_per_share` FLOAT NOT NULL,
  `dividends` bigint(20) NOT NULL,
  `net_cash_flow_generated_by_operating_activities` bigint(20) NOT NULL,
  `market_to_book_value` FLOAT NOT NULL,
  `price_earning_ratio` FLOAT NOT NULL,
  `equity_assets_ratio` FLOAT NOT NULL,
  `return_on_equity` FLOAT NOT NULL,
  PRIMARY KEY (`symbol`, `period`) USING BTREE,
  KEY `idx_symbol` (`symbol`) USING BTREE,
  KEY `idx_period` (`period`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `company_prices` (
  `sessionid` int(11) NOT NULL COMMENT 'unix timestamp',
  `date_` date NOT NULL,
  `symbol` varchar(8) NOT NULL,
  `adjusted_closing_price` float NOT NULL,
  `closing_price` float NOT NULL,
  `change_` float NOT NULL,
  `opening_price` float NOT NULL,
  `base_price` float NOT NULL,
  `high` float NOT NULL,
  `low` float NOT NULL,
  `capital_listed_for_trading` bigint(20) NOT NULL,
  `market_cap` bigint(20) NOT NULL,
  `turnover` int(11) NOT NULL,
  `volume` int(11) NOT NULL,
  `trans` smallint(6) NOT NULL,
  PRIMARY KEY (`symbol`,`date_`) USING BTREE,
  KEY `idx_sessionid` (`sessionid`) USING BTREE,
  KEY `idx_date` (`date_`),
  KEY `idx_symbol` (`symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `bond_prices` (
  `sessionid` int(11) NOT NULL COMMENT 'unix timestamp',
  `date_` date NOT NULL,
  `symbol` varchar(8) NOT NULL,
  `adjusted_closing_price` float NOT NULL,
  `closing_price` float NOT NULL,
  `change_` float NOT NULL,
  `gross_yield_to_maturity` float NOT NULL,
  `opening_price` float NOT NULL,
  `base_price` float NOT NULL,
  `high` float NOT NULL,
  `low` float NOT NULL,
  `capital_listed_for_trading` bigint(20) NOT NULL,
  `market_cap` bigint(20) NOT NULL,
  `turnover` bigint(20) NOT NULL,
  `volume` int(11) NOT NULL,
  `trans` smallint(6) NOT NULL,
  PRIMARY KEY (`symbol`,`date_`) USING BTREE,
  KEY `idx_sessionid` (`sessionid`) USING BTREE,
  KEY `idx_date` (`date_`),
  KEY `idx_symbol` (`symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `fund_prices` (
  `sessionid` int(11) NOT NULL COMMENT 'unix timestamp',
  `date_` date NOT NULL,
  `symbol` varchar(8) NOT NULL,
  `purchase_price` float NOT NULL,
  `redemption_price` float NOT NULL,
  `change_` float NOT NULL,
  `sales_load` float NOT NULL,
  `management_fee` float NOT NULL,
  `asset_value` bigint(20) NOT NULL,
  `trustee_fee` float NOT NULL,
  PRIMARY KEY (`symbol`,`date_`) USING BTREE,
  KEY `idx_sessionid` (`sessionid`) USING BTREE,
  KEY `idx_date` (`date_`),
  KEY `idx_symbol` (`symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `currency` (
  `date_` date NOT NULL,
  `currency_basket` float NOT NULL,
  `USD` float NOT NULL,
  `DEM` float NOT NULL,
  `GBP` float NOT NULL,
  `FF` float NOT NULL,
  `JPY` float NOT NULL,
  `NLG` float NOT NULL,
  `CHF` float NOT NULL,
  `SEK` float NOT NULL,
  `NOK` float NOT NULL,
  `DKK` float NOT NULL,
  `FIM` float NOT NULL,
  `CAD` float NOT NULL,
  `AUD` float NOT NULL,
  `ZAR` float NOT NULL,
  `BEF` float NOT NULL,
  `ATS` float NOT NULL,
  `LIT` float NOT NULL,
  `IEP` float NOT NULL,
  `ESP` float NOT NULL,
  `EUR` float NOT NULL,
  `JOD` float NOT NULL,
  `LBP` float NOT NULL,
  `EGP` float NOT NULL,
  PRIMARY KEY (`date_`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `pagerank` (
  `sessionid` int(11) NOT NULL COMMENT 'unix timestamp',
  `date_` date NOT NULL,
  `symbol` varchar(8) NOT NULL,
  `pagerank` float NOT NULL,
  PRIMARY KEY (`date_`,`symbol`) USING BTREE,
  KEY `idx_sessionid` (`sessionid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `portfolio` (
  `symbol` varchar(8) NOT NULL,
  `date_` date NOT NULL,
  `qty` float NOT NULL,
  `purchase_price` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `keywords` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `sessionid` int(11) NOT NULL COMMENT 'unix timestamp',
  `keyword` varchar(50),
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE (`keyword`),
  KEY `idx_sessionid` (`sessionid`) USING BTREE,
  KEY `idx_keyword` (`keyword`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `company_keywords` (
  `sessionid` int(11) NOT NULL COMMENT 'unix timestamp',
  `symbol` varchar(8) NOT NULL,
  `keyword_id` INT NOT NULL,
  `x` smallint(6) NOT NULL,
  `y` smallint(6) NOT NULL,
  PRIMARY KEY (`keyword_id`, `symbol`) USING BTREE,
  KEY `idx_sessionid` (`sessionid`) USING BTREE,
  KEY `idx_symbol` (`symbol`) USING BTREE,
  FOREIGN KEY (`keyword_id`) REFERENCES keywords(`id`) 
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `watching` (
  `symbol` varchar(8) NOT NULL,
  PRIMARY KEY (`symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `new_portfolio` (
  `symbol` varchar(8) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `markets` (
    `sessionid` int(11) NOT NULL COMMENT "unix timestamp",
    `name` varchar(50) NOT NULL,
    PRIMARY KEY (`name`) USING BTREE
) engine=innodb default charset=utf8;

CREATE TABLE `market_shares` (
    `sessionid` int(11) NOT NULL COMMENT "unix timestamp",
    `market_name` varchar(50) NOT NULL,
    `symbol` varchar(8) NOT NULL,
    PRIMARY KEY (`market_name`,`symbol`) USING BTREE
) engine=innodb default charset=utf8;

CREATE TABLE `market_data` (
  `sessionid` int(11) NOT NULL COMMENT 'unix timestamp',
  `date_` date NOT NULL,
  `name` varchar(50) NOT NULL,
  `base_price` float NOT NULL,
  `opening_price` float NOT NULL,
  `closing_price` float NOT NULL,
  `high` float NOT NULL,
  `low` float NOT NULL,
  `market_cap` bigint(20) NOT NULL,
  PRIMARY KEY (`name`,`date_`) USING BTREE,
  KEY `idx_sessionid` (`sessionid`) USING BTREE,
  KEY `idx_date` (`date_`),
  KEY `idx_symbol` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `news_articles` (
    `id` INT NOT NULL AUTO_INCREMENT,
    `sessionid` int(11) NOT NULL COMMENT "unix timestamp",
    `date_` date NOT NULL,
    `url` text,
    `url_hash` BIGINT NOT NULL,
    `headline` text,
    `summary` text,
    `content` text,
    PRIMARY KEY (`id`) USING BTREE,
    KEY `idx_sessionid` (`sessionid`) USING BTREE,
    UNIQUE (`url_hash`)
) engine=innodb default charset=utf8;

CREATE TABLE `symbol_articles` (
    `sessionid` int(11) NOT NULL COMMENT "unix timestamp",
    `symbol` varchar(8) NOT NULL,
    `article_id` INT NOT NULL,
    PRIMARY KEY (`symbol`, `article_id`) USING BTREE,
    FOREIGN KEY (`article_id`) REFERENCES news_articles(`id`) ,
    KEY `idx_sessionid` (`sessionid`) USING BTREE
) engine=innodb default charset=utf8;

CREATE TABLE `weather` (
    `date_` date NOT NULL,
    `sessionid` int(11) NOT NULL COMMENT 'unix timestamp',
    `temp_high` INT NOT NULL,
    `temp_avg` INT NOT NULL,
    `temp_low` INT NOT NULL,
    `dew_high` INT NOT NULL,
    `dew_avg` INT NOT NULL,
    `dew_low` INT NOT NULL,
    `hum_high` INT NOT NULL,
    `hum_avg` INT NOT NULL,
    `hum_low` INT NOT NULL,
    `pres_high` INT NOT NULL,
    `pres_avg` INT NOT NULL,
    `pres_low` INT NOT NULL,
    `vis_high` INT NOT NULL,
    `vis_avg` INT NOT NULL,
    `vis_low` INT NOT NULL,
    `wind_high` INT NOT NULL,
    `wind_avg` INT NOT NULL,
    `wind_low` INT NOT NULL,
    `precip` float NOT NULL,
    `events` text,
    PRIMARY KEY (`date_`) USING BTREE,
    KEY `idx_sessionid` (`sessionid`) USING BTREE
) engine=innodb default charset=utf8;




CREATE OR REPLACE
VIEW company_news
AS SELECT s.symbol, a.*
FROM news_articles a
INNER JOIN symbol_articles s ON s.article_id = a.id;

CREATE OR REPLACE
VIEW company_entries
AS SELECT c.symbol, c.category,
case c.category
when 'company' then (select count(*) from company_prices where symbol=c.symbol)
when 'bond' then (select count(*) from bond_prices where symbol=c.symbol)
when 'fund' then (select count(*) from fund_prices where symbol=c.symbol)
end
as num
FROM companies c;

CREATE OR REPLACE
VIEW total_entries
AS SELECT sum(num) FROM company_entries;

CREATE OR REPLACE
VIEW session_entries
AS SELECT date_, sessionid,
(select count(*) from market_data where sessionid=s.sessionid) as markets,
(select count(*) from company_prices where sessionid=s.sessionid) as companies,
(select count(*) from bond_prices where sessionid=s.sessionid) as bonds,
(select count(*) from fund_prices where sessionid=s.sessionid) as funds,
(select count(*) from news_articles where sessionid=s.sessionid) as news
FROM sessions s;

CREATE OR REPLACE
VIEW last_session_entries
AS SELECT * FROM session_entries where date_=(select max(date_) from session_entries);

CREATE OR REPLACE
VIEW all_prices
AS SELECT
c.symbol, c.category,
case c.category
when 'company' then p1.date_
when 'bond' then p2.date_
when 'fund' then p3.date_
end
as date_,
case c.category
when 'company' then p1.closing_price
when 'bond' then p2.closing_price
when 'fund' then p3.redemption_price
end
as price,
case c.category
when 'company' then p1.volume
when 'bond' then p2.volume
when 'fund' then 0
end
as volume
FROM companies c
left outer join company_prices p1 on c.symbol = p1.symbol
left outer join bond_prices p2 on c.symbol = p2.symbol
left outer join fund_prices p3 on c.symbol = p3.symbol;

CREATE OR REPLACE
VIEW last_friday
AS SELECT date(if(DATE_FORMAT(curdate(),'%w')>4,date_sub(curdate(),INTERVAL DATE_FORMAT(curdate(),'%w')-5 DAY),date_sub(curdate(),INTERVAL DATE_FORMAT(curdate(),'%w')+2 DAY))) as date_;

CREATE OR REPLACE
VIEW last_thursday
AS SELECT date(if(DATE_FORMAT(curdate(),'%w')>5,date_sub(curdate(),INTERVAL DATE_FORMAT(curdate(),'%w')-6 DAY),date_sub(curdate(),INTERVAL DATE_FORMAT(curdate(),'%w')+3 DAY))) as date_;

CREATE OR REPLACE
VIEW last_thursday_prices
AS SELECT * from all_prices
where date_ = (select date_ from last_thursday);

CREATE OR REPLACE
VIEW portfolio_status
AS SELECT
c.name as company_name,
c.symbol as ticker,
p.date_ as buy_date,
ROUND(p.purchase_price,2) as buy_price,
p.qty as quantity,
ROUND((p.qty*p.purchase_price),2) as buy_value,
ROUND(a2.price,2) as last_price,
ROUND((p.qty*a2.price),2) as current_value,
ROUND((100-(p.purchase_price/a2.price)*100),2) as yield,
ROUND((p.qty*a2.price) - (p.qty*p.purchase_price)) as profit
from portfolio p
inner join companies c on c.symbol=p.symbol
inner join all_prices a2 on a2.symbol=c.symbol and a2.date_ = (select max(date_) from company_prices);

CREATE OR REPLACE
VIEW yearly_change
AS SELECT p1.symbol, p1.price-p2.price
FROM all_prices p1
inner join all_prices p2 on p2.symbol=p1.symbol and (datediff(p1.date_, p2.date_)=365)
where p1.date_=(select max(date_) from all_prices);

CREATE OR REPLACE
VIEW current_pagerank
AS SELECT p.pagerank, c.url, c.name, c.symbol
FROM pagerank p
inner join companies c on c.symbol=p.symbol
where p.date_=(select max(date_) from pagerank)
order by pagerank desc;

CREATE OR REPLACE
VIEW company_financial_data
AS select
f.symbol,
f.period,
ROUND(p.closing_price,2) as price,
f.current_assets as current_assets,
f.current_liabilities as current_liabilities,
ROUND(((p.market_cap)/(p.closing_price)), 0) as shares_outstanding
from financial_statements f
inner join company_prices p on p.symbol=f.symbol
where p.date_ = (select max(date_) from company_prices);

CREATE OR REPLACE
VIEW ncav_data
AS select
company_financial_data.*,
ROUND(((current_assets-current_liabilities)/shares_outstanding),2) as NCAV
from company_financial_data;

CREATE OR REPLACE
VIEW ncav
AS select
ncav_data.*,
ROUND((price*100/ncav),0) as percent
from ncav_data;

CREATE OR REPLACE
VIEW symbol_data
AS select c.*,
n.price, n.current_assets, n.current_liabilities, n.shares_outstanding, n.ncav, n.percent,
f.period, f.total_assets, f.non_current_assets, f.shareholders_equity, f.of_which_minority_interest,
f.long_term_liabilities, f.revenues, f.gross_profit, f.operating_income,
f.income_before_tax, f.net_income, f.income_atributable_to_shareholders,
f.earnings_per_share, f.dividends, f.net_cash_flow_generated_by_operating_activities,
f.market_to_book_value, f.price_earning_ratio, f.equity_assets_ratio, f.return_on_equity,
ifnull(p.pagerank,0) as pagerank
from companies c
left outer join ncav n on n.symbol=c.symbol
left outer join current_pagerank p on p.symbol=c.symbol
left outer join financial_statements f on f.symbol=c.symbol and f.period='June 2012';

create view profit
as
select
sum(buy_value) as buy,
sum(current_value) as val,
sum(current_value)-sum(buy_value) as profit
from portfolio_status;





CREATE PROCEDURE `tase`.`delete_session` (IN session_id INT)
BEGIN
DELETE FROM `sessions` WHERE `sessionid` = session_id;
DELETE FROM `companies` WHERE `sessionid` = session_id;
DELETE FROM `financial_statements` WHERE `sessionid` = session_id;
DELETE FROM `company_prices` WHERE `sessionid` = session_id;
DELETE FROM `bond_prices` WHERE `sessionid` = session_id;
DELETE FROM `fund_prices` WHERE `sessionid` = session_id;
DELETE FROM `pagerank` WHERE `sessionid` = session_id;
DELETE FROM `keywords` WHERE `sessionid` = session_id;
DELETE FROM `company_keywords` WHERE `sessionid` = session_id;
DELETE FROM `market_shares` WHERE `sessionid` = session_id;
DELETE FROM `market_data` WHERE `sessionid` = session_id;
DELETE FROM `symbol_articles` WHERE `sessionid` = session_id;
DELETE FROM `news_articles` WHERE `sessionid` = session_id;
END


