#!/bin/bash -l

module load intel likwid

cd .. && make clean && make

FILENAME="jacobi_comparision_results_$(date +%Y%m%d_%H%M%S).txt"
echo "Starting Benchmark..." > "${FILENAME}"

echo "Jacobi BENCHMARK" | tee -a "${FILENAME}"
echo "Benchmarking across 1 NUMA " | tee -a "${FILENAME}"

for matrix in {matrix_band_small,HPCG-25-25-25,HPCG-72-72-72,HPCG-128-128-128}
do
    echo "---------------------------------------------------------------" | tee -a "${FILENAME}"
    echo "Matrix : ${matrix}" | tee -a "${FILENAME}"
    
    t=18
        echo "Running with threads: ${t}" | tee -a "${FILENAME}"
        echo "Jacobi"
        srun --cpu-freq=2200000-2200000:performance \
             --export=ALL,OMP_NUM_THREADS=${t},OMP_PLACES=cores,OMP_PROC_BIND=close \
             ./benchmark jacobi matrices/${matrix}.mtx >> "${FILENAME}"
        echo "Jacobi Separate"
        srun --cpu-freq=2200000-2200000:performance \
             --export=ALL,OMP_NUM_THREADS=${t},OMP_PLACES=cores,OMP_PROC_BIND=close \
             ./benchmark jacobi_separate matrices/${matrix}.mtx >> "${FILENAME}"
        echo "Jacobi Orphaned"
        srun --cpu-freq=2200000-2200000:performance \
             --export=ALL,OMP_NUM_THREADS=${t},OMP_PLACES=cores,OMP_PROC_BIND=close \
             ./benchmark jacobi_orphaned matrices/${matrix}.mtx >> "${FILENAME}"
    
done

echo "Done. Results saved to ${FILENAME}"