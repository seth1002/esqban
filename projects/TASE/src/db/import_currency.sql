load data local infile '/home/sqba/Projects/TASE/src/db/yazig.csv' 
into table tase.currency 
fields terminated by ',' OPTIONALLY ENCLOSED BY '"'
lines terminated by '\n'
IGNORE 1 LINES
(@date, currency_basket, USD, DEM, GBP, FF, JPY, NLG, CHF, SEK, NOK, DKK, FIM, CAD, AUD, ZAR, BEF, ATS, LIT, IEP, ESP, EUR, JOD, LBP, EGP)
    SET date_ =  STR_TO_DATE(@date, '%d/%m/%Y')
;

