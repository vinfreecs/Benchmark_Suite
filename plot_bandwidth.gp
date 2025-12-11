set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'bandwidth_numa.png'

set datafile separator ","
set title "Memory Bandwidth vs Data Size 1 NUMA Fritz"
set xlabel "Size (MB)"
set ylabel "Bandwidth (MByte/s)"
set grid
set key top right box

set logscale y
set logscale x

plot "results/bandwidths/bandwidth_numa.csv" every ::1 using 1:2 with linespoints lw 2 pt 7 ps 1.5 title "Bandwidth"