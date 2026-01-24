#!/bin/bash -l
module load intel likwid

FILENAME = "axpby_benchmark.csv"

cd .. && make

echo "AXPBY BENCHMARK"
echo "Benchmarking across 1 Socket"

for N in {18,32,54,72}; 
do
    echo " no: of threads $((N))"
    
    srun --cpu-freq=2000000-2000000:performance \
    --export=ALL,OMP_NUM_THREADS=$N,OMP_PLACES=cores,OMP_PROC_BIND=close \
    ./benchmark dot 1000000000 1
done

echo "---------------------------------------------------------------"
