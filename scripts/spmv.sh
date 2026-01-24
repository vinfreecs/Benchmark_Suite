#!/bin/bash -l
module load intel likwid

FILENAME="nv1_10_cores.txt"

cd .. && make clean &&  make

echo "SPMV BENCHMARK" 
echo "Benchmarking across 1 Node"

for t in {18,32,54,72}; 
do
    echo " no: of threads $((t))" 
    
    srun --cpu-freq=2200000-2200000:performance \
    --export=ALL,OMP_NUM_THREADS=${t},OMP_PLACES=cores,OMP_PROC_BIND=spread \
    ./benchmark spmv matrices/kkt_power.mtx
done

echo "---------------------------------------------------------------"
