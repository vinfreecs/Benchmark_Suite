#!/bin/bash -l
module load intel likwid

FILENAME="spmv_benchmark.csv"

cd .. && make

echo "SPMV BENCHMARK"
echo "Benchmarking across 1 Node"

for t in {18,32,54,72}; 
do
    echo " no: of threads $((t))"
    
    srun --cpu-freq=2200000-2200000:performance \
    --export=ALL,OMP_NUM_THREADS=${t},OMP_PLACES=cores,OMP_PROC_BIND=close \
    ./benchmark spmv_mult 1000 5
done

echo "---------------------------------------------------------------"
