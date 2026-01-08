#define PRINT_KERNAL(kernal)                                                   \
  std::cout << "-- BENCHMARKING " << kernal << " -- \n";                       \
  std::cout << "The number of iterations " << iter                             \
            << " the size of the array " << N << "\n";

#define HARNESS(kernal, mult_value, NN)                                        \
  for (int wit = 0; wit < 0; wit++) {                                          \
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

#define TIME_SOLVER(kernal, NN)                                                \
  double start, end;                                                           \
  start = getTimeStamp();                                                      \
  kernal;                                                                      \
  end = getTimeStamp();                                                        \
  double duration = end - start;                                               \
  std::cout << "Time taken : " << duration << "\n";                            \
  std::cout << "Performance : "                                                \
            << ((double)((double)iter * (double)NN)) / (duration * 1e9)        \
            << "GLUP/s \n";

#define PRINT_SPARSE_DETAILS(mat)                                              \
  std::cout << "\nRows : " << mat.rows << " Cols : " << mat.cols               \
            << " nnz : " << mat.nnz << "\n";                                   \
  std::cout << "number of elements in values : " << mat.values.size()          \
            << std::endl;                                                      \
  std::cout << "the size of row_start : " << mat.row_start.size()              \
            << std::endl;                                                      \
  std::cout << "the size of col_idx : " << mat.col_idx.size() << std::endl;    \
  std::cout << "the size of values : " << mat.values.size() << std::endl;
