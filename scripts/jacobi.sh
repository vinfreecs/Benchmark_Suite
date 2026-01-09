#!/bin/bash -l
module load intel likwid

FILENAME="nv1_10_cores.txt"

cd .. && make clean && make

echo "SPMV BENCHMARK" 
echo "Benchmarking across 1 Node"



echo " no: of threads 72" 

srun \
--export=ALL,OMP_NUM_THREADS=72,OMP_PLACES=cores,OMP_PROC_BIND=close \
./benchmark gauss_seidel


echo "---------------------------------------------------------------"
