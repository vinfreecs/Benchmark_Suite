#!/bin/bash -l

module load likwid


likwid-bench -t load_avx -w M0:1GB -w M0:1GB | tail -23 >> results/bandwidth.txt
