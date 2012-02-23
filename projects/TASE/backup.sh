#/bin/sh

NOW=$(date +"%b-%d-%Y")
LOGFILE="tase_backup_$NOW.gz"

echo 'Backing up:' $LOGFILE

tar -cvzpf ./backups/$LOGFILE ./ --exclude=./db_backups --exclude=./backups
