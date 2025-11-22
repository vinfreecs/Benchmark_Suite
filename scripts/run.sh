#!/bin/bash

KERNEL="$1"
N="$2"
ITER="$3"

if [ -z "$KERNEL" ] || [ -z "$N" ] || [ -z "$ITER" ]; then
    echo "Usage: $0 <kernel> <N_elements> <repetitions>"
    exit 1
fi

for t in {18,32,54,72}; do
    echo " no: of threads $t"

    srun --cpu-freq=2400000-2400000:performance \
         --export=ALL,OMP_NUM_THREADS=$t,OMP_PLACES=cores,OMP_PROC_BIND=close \
         .././benchmark "$KERNEL" "$N" "$ITER"
done
