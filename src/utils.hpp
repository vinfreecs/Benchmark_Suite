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

#define TIME_SOLVER(kernal, NN, mat)                                           \
  double start, end;                                                           \
  start = getTimeStamp();                                                      \
  kernal;                                                                      \
  end = getTimeStamp();                                                        \
  double duration = end - start;                                               \
  std::cout << "Time taken : " << duration << "\n";                            \
  std::cout << "Performance : " << ((double)NN) / (duration) << "iter/s \n";   \
  std::cout << "Bandwidth : "                                                  \
            << ((mat.values.size() * 8 + mat.col_idx.size() * 4 +              \
                 mat.row_start.size() * 4) *                                   \
                NN) /                                                          \
                   (duration * 1e9)                                            \
            << std::endl;                                                      \
  std::cout << "Performance in Flops/s : " << std::endl;

#define PRINT_SPARSE_DETAILS(mat)                                              \
  std::cout << "\nRows : " << mat.rows << " Cols : " << mat.cols               \
            << " nnz : " << mat.nnz << "\n";                                   \
  std::cout << "number of elements in values : " << mat.values.size()          \
            << std::endl;                                                      \
  std::cout << "the size of row_start : " << mat.row_start.size()              \
            << std::endl;                                                      \
  std::cout << "the size of col_idx : " << mat.col_idx.size() << std::endl;    \
  std::cout << "the size of values : " << mat.values.size() << std::endl;      \
  std::cout << "the size of values : "                                         \
            << mat.values.size() * 8 / (1024 * 1024) << std::endl;             \
  std::cout << "the size of col_idx : "                                        \
            << mat.col_idx.size() * 4 / (1024 * 1024) << std::endl;            \
  std::cout << "the size of row_start : "                                      \
            << mat.row_start.size() * 4 / (1024 * 1024) << std::endl;          \
  std::cout << "the total size taken up by the sparse matrix : "               \
            << mat.values.size() * 8 / (1024 * 1024) +                         \
                   mat.col_idx.size() * 4 / (1024 * 1024) +                    \
                   mat.row_start.size() * 4 / (1024 * 1024)                    \
            << "MB" << std::endl;
