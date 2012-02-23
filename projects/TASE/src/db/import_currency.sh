#/bin/sh

wget -N 'http://www.bankisrael.gov.il/deptdata/mth/yazig.zip'

unzip -o yazig.zip

xls2csv -x YAZIG.XLS -c yazig.csv

mysql -usqba -p12345 tase < import_currency.sql

rm yazig.zip
rm yazig.csv
rm YAZIG.XLS



