#!/bin/bash -l

module load likwid

# Define the output file name
outfile="bandwidth_results_5.csv"

# Clear file and write Header
echo "Size_GB,Bandwidth_MByte_s" > $outfile

# Loop through sizes with units
for size in 10KB 30KB 50KB 100KB 200KB 250KB 350KB 432KB 500KB 1MB 3MB 7MB 12MB 16MB 25MB 40MB 60MB 100MB 250MB 500MB 750MB 1GB; do

    echo "Running with size: ${size}..."

    # --- STEP 1: Normalize Size to GB ---
    # We strip the unit suffix and calculate based on KB, MB, or GB
    number=${size%??} # Removes last 2 chars (KB, MB, GB)
    unit=${size: -2}  # Grabs last 2 chars
    
    # Calculate GB based on the unit found
    if [ "$unit" == "KB" ]; then
        gb_val=$(awk "BEGIN {print $number/1024/1024}")
    elif [ "$unit" == "MB" ]; then
        gb_val=$(awk "BEGIN {print $number/1024}")
    else 
        # Assume GB
        gb_val=$number
    fi

    # --- STEP 2: Run Benchmark ---
    # We pass the CALCULATED gb_val to awk for the CSV
    likwid-bench -t load_avx -w M0:${size} -w M0:${size} | \
    awk -v gb="$gb_val" '/MByte\/s:/ { printf "%.6f,%s\n", gb, $NF }' >> $outfile

done

echo "Done. Saved to $outfile"