SELECT name, latitude, longitude, type INTO OUTFILE '/tmp/marinas.csv'
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"'
ESCAPED BY '\\'
LINES TERMINATED BY '\n'
FROM marinas
