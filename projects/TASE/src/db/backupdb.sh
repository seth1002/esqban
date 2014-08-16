#/bin/sh

NOW=$(date +"%b-%d-%Y")
LOGFILE="/media/Maxtor/tase_backup_$NOW.sql.gz"

echo 'Backing up:' $LOGFILE

#mysqldump -h localhost -uroot -p tase | pv | gzip -9 > $LOGFILE
mysqldump -h localhost -uroot -p tase | gzip -9 > $LOGFILE

