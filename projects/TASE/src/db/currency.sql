load data local infile '/home/sqba/workspace/TASE/exchange/YAZIG.csv' 
into table tase.currency 
fields terminated by ',' 
lines terminated by '\n' 
(
@date_, 
currency_basket, 
USD, 
DEM, 
GBP,
FF,
JPY,
NLG,
CHF,
SEK,
NOK,
DKK,
FIM,
CAD,
AUD,
ZAR,
BEF,
ATS,
LIT,
IEP,
ESP,
EUR,
JOD,
LBP,
EGP) 
set 
date_ = str_to_date(@date_, '%d/%m/%Y');



