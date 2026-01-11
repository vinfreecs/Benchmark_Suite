#!/bin/bash -l
module load intel likwid

cd .. && make clean && make

echo "Gauss Seidel BENCHMARK" 
echo " no: of threads 72" 

srun \
--export=ALL,OMP_NUM_THREADS=72,OMP_PLACES=cores,OMP_PROC_BIND=close \
./benchmark gauss_seidel matrices/HPCG-128-128-128.mtx


echo "---------------------------------------------------------------"
