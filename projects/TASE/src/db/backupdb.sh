#/bin/sh

NOW=$(date +"%b-%d-%Y")
LOGFILE="tase_backup_$NOW.sql.gz"

echo 'Backing up:' $LOGFILE

#mysqldump -h 192.168.2.106 -uroot -p tase | pv | gzip -9 > $LOGFILE
mysqldump -h 192.168.2.106 -uroot -p tase | gzip -9 > $LOGFILE

