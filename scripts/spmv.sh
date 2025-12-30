#!/bin/bash -l
module load intel likwid

FILENAME="nv1_10_cores.txt"

cd .. && make

echo "SPMV BENCHMARK" 
echo "Benchmarking across 1 Node"

for t in {1,2,3,4,5,6,7,8,9,10}; 
do
    echo " no: of threads $((t))" 
    
    srun --cpu-freq=2200000-2200000:performance \
    --export=ALL,OMP_NUM_THREADS=${t},OMP_PLACES=cores,OMP_PROC_BIND=close \
    ./benchmark spmv_mult 1000 500 
done

echo "---------------------------------------------------------------"
