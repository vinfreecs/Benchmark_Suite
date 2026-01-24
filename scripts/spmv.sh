#!/bin/bash -l
module load intel likwid

FILENAME="nv1_10_cores.txt"

cd .. && make

echo "SPMV BENCHMARK" 
echo "Benchmarking across 1 Node"

for t in {1..72}; 
do
    echo " no: of threads $((t))" 
    
    srun --cpu-freq=2200000-2200000:performance \
    --export=ALL,OMP_NUM_THREADS=${t},OMP_PLACES=cores,OMP_PROC_BIND=spread \
    ./benchmark spmv 1000 matrices/matrix_band_small.mtx
done

echo "---------------------------------------------------------------"
