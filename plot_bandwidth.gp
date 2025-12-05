# Output Settings
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'bandwidth_plot_5.png'

# Formatting
set datafile separator ","
set title "Memory Bandwidth vs Data Size"
set xlabel "Size (GB)"
set ylabel "Bandwidth (MByte/s)"
set grid
set key top right box

# Log scale makes the huge drop at the start easier to see (Optional)
set logscale y 
set logscale x

# Plot command skipping the header row
plot "scripts/bandwidth_results_5.csv" every ::1 using 1:2 with linespoints lw 2 pt 7 ps 1.5 title "Bandwidth"