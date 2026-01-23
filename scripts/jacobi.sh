#!/bin/bash -l
module load intel likwid

FILENAME="nv1_10_cores.txt"

cd .. && make clean && make

echo "Jacobi BENCHMARK" 
echo " no: of threads 72" 

srun --cpu-freq=2200000-2200000:performance \
--export=ALL,OMP_NUM_THREADS=72,OMP_PLACES=cores,OMP_PROC_BIND=close \
./benchmark jacobi matrices/HPCG-128-128-128.mtx


echo "---------------------------------------------------------------"
