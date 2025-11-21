

# for t in {18,32,54,72}
# do
# # Set OMP environment variables
# export OMP_NUM_THREADS=$t
# export OMP_PLACES=cores
# export OMP_PROC_BIND=close

# # Run the benchmark, simplifying the --cpu-freq flag if the previous one failed
# srun --cpu-freq=2400000-2400000:performance ./benchmark axpby 10000000 10
# done

#___________________compile befor running this shell___________________

for t in {1..72}; 
do
    echo " no: of threads $((t))"
    
    srun --cpu-freq=2400000-2400000:performance \
    --export=ALL,OMP_NUM_THREADS=$t,OMP_PLACES=cores,OMP_PROC_BIND=close \
    ./benchmark dot 1000000 10
done