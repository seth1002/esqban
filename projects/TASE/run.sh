#/bin/sh

svn update .
svn update /var/www/tase

cd src

./run.sh

