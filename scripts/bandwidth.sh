#!/bin/bash -l

module load likwid

outfile="bandwidth_results_5.csv"

echo "Size_GB,Bandwidth_MByte_s" > $outfile

for size in 10KB 30KB 50KB 100KB 200KB 250KB 350KB 432KB 500KB 1MB 3MB 7MB 12MB 16MB 25MB 40MB 60MB 100MB 250MB 500MB 750MB 1GB; do

    echo "Running with size: ${size}..."

    number=${size%??} 
    unit=${size: -2}  
    
    if [ "$unit" == "KB" ]; then
        gb_val=$(awk "BEGIN {print $number/1024/1024}")
    elif [ "$unit" == "MB" ]; then
        gb_val=$(awk "BEGIN {print $number/1024}")
    else 
        gb_val=$number
    fi

#one socket 
    likwid-bench -t load_avx -w M0:${size} -w M1:${size} | \
    awk -v gb="$gb_val" '/MByte\/s:/ { printf "%.6f,%s\n", gb, $NF }' >> $outfile

done

echo "Done. Saved to $outfile"