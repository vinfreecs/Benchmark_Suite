set datafile separator ","

set title "DOT Performance for N = 100,000,000"
set xlabel "Threads"
set ylabel "Performance (GIt/s)"

set grid
set key left top

set term pngcairo size 1200,800
set output "results/dot_performance_mem_bound.png"

plot "results/dot_results_mem.csv" using 1:3 with linespoints lw 2 pt 7 title "Performance (GIt/s)"
