#!/bin/bash
#
# Script to parse AXPBY benchmark output and convert it to a CSV file
# with a custom two-line header (info header + column header).
#
# Usage:
#   cat new_raw_benchmark_data.txt | ./parse_benchmark_data_v2.sh

output_file="dot_results.csv"

# The core AWK command for parsing the data
awk '
BEGIN {
    # Set the Output Field Separator for CSV format
    OFS = ","
    
    # 1. Print the custom kernel/size/iter header line (inferred from the input data: AXPBY, N=1000000, iter=10)
    print "AXPBY", 1000000, 10
    
    # 2. Print the column header row
    print "Threads", "Time (s)", "Performance (GIt/s)"
    
    # Initialize variables for thread block data
    threads = ""
    time = ""
    perf = ""
}

# 1. Look for the "no: of threads" line
/no: of threads/ {
    # The thread count is the 4th space-separated field
    threads = $4
}

# 2. Look for the "Time taken" line
/Time taken/ {
    # Time is the 3rd space-separated field
    time = $4
}

# 3. Look for the "Performance" line (which completes a data set)
/Performance/ {
    # Performance is the 3rd field, which includes the unit ("MIt/s").
    # Use sub to remove the unit, leaving only the numerical value.
    perf = $3
    sub(/MIt\/s/, "", perf)
    perf_giga = perf/1000
    
    # Now that we have all three values, print the full CSV record
    print threads, time, perf_giga
    
    # Reset variables for the next thread block
    threads = ""
    time = ""
    perf = ""
}

# END block is optional here
' > "$output_file"

echo "Data successfully parsed and written to $output_file"