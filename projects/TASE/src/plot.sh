#/bin/sh

symbol='ADO'

python plot.py "$symbol"

cat plot-load.conf | gnuplot

