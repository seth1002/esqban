#/bin/sh

rm yazig.csv
rm yazigmizt.xls

#wget -N 'http://www.bankisrael.gov.il/deptdata/mth/yazig.zip'
wget -N 'http://www.boi.org.il/he/Markets/Documents/yazigmizt.xls'

#unzip -o yazig.zip

#mv YAZIG.XLS yazig.xls

#xls2csv yazig.xls > yazig.csv
#xls2csv -x yazig.xls -f%d/%m/%Y > yazig.csv
xls2csv -x yazigmizt.xls -c yazig.csv

#mysql -usqba -p12345 tase < import_currency.sql
mysql -utase -p12345 --local-infile tase < import_currency.sql

#rm yazig.zip



