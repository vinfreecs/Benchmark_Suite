# Set output to a PNG image
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'performance_plot_1node.png'

# Graph labels and title
set title "Performance vs Threads(1 Fritz Node 10 cores) SPMV"
set xlabel "Threads"
set ylabel "Performance (GFlops/s)"
set grid

set xrange [0:*]
# Legend (key) configuration
set key top left

# Set the input file format
set datafile separator ","

# Plotting command
# 'skip 4' ignores the first 4 lines (metadata + header)
# 'using 1:3' takes the 1st column (threads) as X and 3rd (performance) as Y
# Gnuplot typically ignores the text suffix ('GFlops/s') automatically when reading numbers.

plot "results/sparse/garon2_72_cores.txt" using 1:3 skip 4 with linespoints lw 2 pt 7 title "garon2", \
     "results/sparse/nv1_72_cores.txt" using 1:3 skip 4 with linespoints lw 2 pt 7 title "nv_1", \
     "results/sparse/kkt_power_72_cores.txt" using 1:3 skip 4 with linespoints lw 2 pt 7 title "kkt_power"