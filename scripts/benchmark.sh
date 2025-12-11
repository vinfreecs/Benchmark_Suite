#!/bin/bash -l
module load intel likwid

FILENAME = "axpby_benchmark.csv"

make

echo "AXPBY BENCHMARK"
echo "Benchmarking across 1 Socket"

for N in {18,32,54,72}; 
do
    echo " no: of threads $((t))"
    
    srun --cpu-freq=2200000-2200000:performance \
    --export=ALL,OMP_NUM_THREADS=36,OMP_PLACES=cores,OMP_PROC_BIND=close \
    ./benchmark axpby 1000000000 10
done

echo"---------------------------------------------------------------"
