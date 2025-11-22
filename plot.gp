set datafile separator ","

set title "AXPBY Performance for N = 100,000,000"
set xlabel "Threads"
set ylabel "Performance (GFlops/s)"

set grid
set key left top

set term pngcairo size 1200,800
set output "results/axpby_performance_mem_bound.png"

MAX = 41;

plot \
    "results/axpby_results_mem.csv" using 1:3 with linespoints lw 2 pt 7 title "Performance (GFLops/s)", \
    MAX with lines lw 3 dt 2 lc rgb "red" title sprintf("Roofline = %.1f GFLOP/s", MAX)
