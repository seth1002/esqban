
/*drop table `tmp_bond_prices`;*/
CREATE TABLE `tmp_bond_prices` (
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

insert into tmp_bond_prices select * from bond_prices;

update tmp_bond_prices
set
adjusted_closing_price=adjusted_closing_price/100,
closing_price=closing_price/100,
opening_price=opening_price/100,
base_price=base_price/100,
high=high/100,
low=low/100,
market_cap=market_cap*1000;



/*drop table `tmp_financial_statements`;*/
CREATE TABLE `tmp_financial_statements` (
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
  `earnings_per_share` bigint(20) NOT NULL,
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

insert into tmp_financial_statements select * from financial_statements;

update tmp_financial_statements
set
total_assets=total_assets*1000,
current_assets=current_assets*1000,
non_current_assets=non_current_assets*1000,
shareholders_equity=shareholders_equity*1000,
of_which_minority_interest=of_which_minority_interest*1000,
current_liabilities=current_liabilities*1000,
long_term_liabilities=long_term_liabilities*1000,
revenues=revenues*1000,
gross_profit=gross_profit*1000,
operating_income=operating_income*1000,
income_before_tax=income_before_tax*1000,
net_income=net_income*1000,
income_atributable_to_shareholders=income_atributable_to_shareholders*1000,
dividends=dividends*1000,
net_cash_flow_generated_by_operating_activities=net_cash_flow_generated_by_operating_activities*1000;



/*drop table `tmp_company_prices`;*/
CREATE TABLE `tmp_company_prices` (
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

insert into tmp_company_prices select * from company_prices;

update tmp_company_prices
set
adjusted_closing_price=adjusted_closing_price/100,
closing_price=closing_price/100,
opening_price=opening_price/100,
base_price=base_price/100,
high=high/100,
low=low/100,
market_cap=market_cap*1000;



/*drop table `tmp_fund_prices`;*/
/*
CREATE TABLE `tmp_fund_prices` (
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

insert into tmp_fund_prices select * from fund_prices;

update tmp_fund_prices
set
purchase_price=purchase_price*100,
redemption_price=redemption_price*100;
*/

