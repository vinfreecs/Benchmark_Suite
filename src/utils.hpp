#define PRINT_KERNAL(kernal)                                                   \
  std::cout << "-- BENCHMARKING " << kernal << " -- \n";                       \
  std::cout << "The number of iterations " << iter                             \
            << " the size of the array " << N << "\n";

#define HARNESS(kernal, mult_value, NN)                                        \
  for (int wit = 0; wit < warmupIter; wit++) {                                 \
    kernal;                                                                    \
  }                                                                            \
  double start, end;                                                           \
  start = getTimeStamp();                                                      \
  for (int it = 0; it < iter; it++) {                                          \
    kernal;                                                                    \
  }                                                                            \
  end = getTimeStamp();                                                        \
  double duration = end - start;                                               \
  std::cout << "Time taken : " << duration << "\n";                            \
  std::cout << "Performance : "                                                \
            << ((double)((double)iter * (double)NN) * mult_value) /            \
                   (duration * 1e9)                                            \
            << "GFlops/s \n";
