set datafile separator ","

set title "DOT Performance for N = 1,000,000"
set xlabel "Threads"
set ylabel "Performance (GIt/s)"

set grid
set key left top

set term pngcairo size 1200,800
set output "dot_performance.png"

plot "scripts/dot_results.csv" using 1:3 with linespoints lw 2 pt 7 title "Performance (GIt/s)"
