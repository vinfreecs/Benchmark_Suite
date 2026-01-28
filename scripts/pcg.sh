#!/bin/bash -l
module load intel likwid

FILENAME="pcg_72_cores.txt"

cd .. && make clean && make
echo "---------------------------------------------------------------"
echo "PCG BENCHMARK" 
echo " no: of threads 72" 
echo "============================================"

echo "preconditioner : jacobi"

srun --cpu-freq=2200000-2200000:performance \
--export=ALL,OMP_NUM_THREADS=72,OMP_PLACES=cores,OMP_PROC_BIND=close \
./benchmark pcg jacobi matrices/HPCG-128-128-128.mtx
echo "============================================"
echo "============================================"

echo "preconditioner : symmetric guass seidel"

srun --cpu-freq=2200000-2200000:performance \
--export=ALL,OMP_NUM_THREADS=72,OMP_PLACES=cores,OMP_PROC_BIND=close \
./benchmark pcg sgs matrices/HPCG-128-128-128.mtx
echo "============================================"

#  srun --cpu-freq=2200000-2200000:performance likwid-pin  -q -c N:0-71 .././benchmark cg ../matrices/HPCG-128-128-128.mtx



echo "---------------------------------------------------------------"
