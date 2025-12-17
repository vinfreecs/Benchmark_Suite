#!/bin/bash -l

module load intel
cd ..
make clean

make

./benchmark spmv 100 1

make clean