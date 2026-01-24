#!/bin/bash -l
module load intel likwid

FILENAME="nv1_10_cores.txt"

cd .. && make clean && make

echo "CG BENCHMARK" 
echo " no: of threads 72" 

srun --cpu-freq=2200000-2200000:performance \
--export=ALL,OMP_NUM_THREADS=72,OMP_PLACES=cores,OMP_PROC_BIND=close \
./benchmark cg matrices/HPCG-128-128-128.mtx

#  srun --cpu-freq=2200000-2200000:performance likwid-pin  -q -c N:0-71 .././benchmark cg ../matrices/HPCG-128-128-128.mtx



echo "---------------------------------------------------------------"
