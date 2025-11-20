
module load intel likwid

cd build && cmake ..

make

echo "AXPBY BENCHMARK"

echo "Benchmarking across 1 Fritz Node"
for t in {1..72}; 
do
    echo " no: of threads $t"
    srun --cpu-freq=2400000-2400000:performance \
         --export=ALL,OMP_NUM_THREADS=$t,OMP_PLACES=cores,OMP_PROC_BIND=close \
        ./benchmark axpby 10000
done

echo "Benchmarking across 1 Numa Domain in a Fritz Node "
for t in {1..18}; 
do
    echo " no: of threads $t"
    srun --cpu-freq=2400000-2400000:performance \
         --export=ALL,OMP_NUM_THREADS=$t,OMP_PLACES=cores,OMP_PROC_BIND=close \
        ./benchmark axpby 10000
done