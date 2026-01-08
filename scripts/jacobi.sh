#!/bin/bash -l
module load intel likwid

FILENAME="nv1_10_cores.txt"

cd .. && make

echo "SPMV BENCHMARK" 
echo "Benchmarking across 1 Node"

for t in {71,72}; 
do
    echo " no: of threads $((t))" 
    
    srun \
    --export=ALL,OMP_NUM_THREADS=${t},OMP_PLACES=cores,OMP_PROC_BIND=close \
    ./benchmark jacobi
done

echo "---------------------------------------------------------------"
