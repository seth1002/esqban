#/bin/sh

cd /home/sqba/Projects/TASE
svn update .
svn update /var/www/tase

cd src

./run.sh

