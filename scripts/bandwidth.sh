#!/bin/bash -l

module load likwid

outfile="bandwidth_numa.csv"

# Header
echo "Size_MB,Bandwidth_MByte_s" > $outfile

# --- Sizes adapted for Single NUMA Domain (approx 18 cores) ---
l1_sizes="10KB 25KB 40KB 80KB 150KB 300KB 600KB 1000KB 1500KB 2000KB"
l2_sizes="3MB 5MB 8MB 12MB 16MB 18MB 20MB 22MB 24MB 26MB 30MB"
l3_sizes="35MB 40MB 45MB 50MB 55MB 60MB 65MB 70MB 80MB 90MB"
mem_sizes="100MB 120MB 150MB 200MB 300MB 500MB 800MB 1GB 1.5GB"

all_sizes="$l1_sizes $l2_sizes $l3_sizes $mem_sizes"

for size in $all_sizes; do

    echo "Running with size: ${size}..."

    number=${size%??} 
    unit=${size: -2}  
    
    # Calculate MB value for X-axis
    if [ "$unit" == "KB" ]; then
        mb_val=$(awk "BEGIN {print $number/1024}")
    elif [ "$unit" == "MB" ]; then
        mb_val=$number
    else 
        # GB to MB conversion
        mb_val=$(awk "BEGIN {print $number*1024}")
    fi

    # --- EXECUTION ---
    # -W N:0:${size} runs on all cores inside NUMA Node 0
    likwid-bench -t load_avx -w M0:${size} | \
    grep "MByte/s" | \
    tail -n 1 | \
    awk -v x_axis="$mb_val" '{ printf "%.6f,%s\n", x_axis, $NF }' >> $outfile

done

echo "Done. Results saved to $outfile"