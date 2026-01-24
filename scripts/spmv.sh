#!/bin/bash -l

module load intel likwid

cd .. && make clean && make

FILENAME="benchmark_results_$(date +%Y%m%d_%H%M%S).txt"
echo "Starting Benchmark..." > "${FILENAME}"

echo "SPMV BENCHMARK" | tee -a "${FILENAME}"
echo "Benchmarking across 1 NUMA Domain " | tee -a "${FILENAME}"

matrix=HPCG-128-128-128
# for matrix in {HPCG-128-128-128}
# do
    echo "---------------------------------------------------------------" | tee -a "${FILENAME}"
    echo "Matrix : ${matrix}" | tee -a "${FILENAME}"
    
    for t in {44..72}; 
    do
        echo "Running with threads: ${t}" | tee -a "${FILENAME}"
        
        srun --cpu-freq=2200000-2200000:performance \
             --export=ALL,OMP_NUM_THREADS=${t},OMP_PLACES=cores,OMP_PROC_BIND=close \
             ./benchmark spmv matrices/${matrix}.mtx >> "${FILENAME}"
    done
# done

echo "Done. Results saved to ${FILENAME}"