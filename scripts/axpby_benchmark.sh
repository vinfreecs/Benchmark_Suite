#!/bin/bash -l
module load intel likwid

FILENAME = "axpby_benchmark.csv"


mkdir ../build && cd ../build && cmake ..

make

echo "AXPBY BENCHMARK"
echo "Benchmarking across 1 Fritz Node"

for t in {0..71}; 
do
    echo " no: of threads $((t+1))"
    srun --cpu-freq=2400000-2400000:performance \
     likwid-pin -c 0-$t -q ./benchmark axpby 10000 50000 
done

echo"---------------------------------------------------------------"

echo "Benchmarking across 1 Numa Domain in a Fritz Node "
for t in {0..17}; 
do
    echo " no: of threads $((t+1))"
    srun --cpu-freq=2400000-2400000:performance \
     likwid-pin -c 0-$t -q ./benchmark axpby 10000 50000 
done