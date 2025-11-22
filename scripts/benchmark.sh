#!/bin/bash -l
module load intel likwid

FILENAME = "axpby_benchmark.csv"

cd ../build && cmake ..

make

echo "AXPBY BENCHMARK"
echo "Benchmarking across 1 Fritz Node"

for t in {18,32,54,72}; 
do
    echo " no: of threads $((t))"
    
    srun --cpu-freq=2400000-2400000:performance \
    --export=ALL,OMP_NUM_THREADS=$t,OMP_PLACES=cores,OMP_PROC_BIND=close \
    ./benchmark axpby 10000000 10
done

echo"---------------------------------------------------------------"
