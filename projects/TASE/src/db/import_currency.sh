#/bin/sh

#wget -N 'http://www.bankisrael.gov.il/deptdata/mth/yazig.zip'
wget -N 'http://www.boi.org.il/he/Markets/Documents/yazig.zip'

unzip -o yazig.zip

#xls2csv YAZIG.XLS > yazig.csv
#xls2csv -x YAZIG.XLS -f%d/%m/%Y > yazig.csv
xls2csv -x YAZIG.XLS -c yazig.csv

#mysql -usqba -p12345 tase < import_currency.sql
mysql -utase -p12345 --local-infile tase < import_currency.sql

rm yazig.zip
rm yazig.csv
rm YAZIG.XLS



