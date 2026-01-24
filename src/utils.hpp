#define PRINT_KERNAL(kernal)                                                   \
  std::cout << "-- BENCHMARKING " << kernal << " -- \n";

#define HARNESS(kernal, mult_value, NN)                                        \
  for (int wit = 0; wit < iter; wit++) {                                       \
    kernal;                                                                    \
  }                                                                            \
  double start, end;                                                           \
  double duration = 0;                                                         \
  while (duration < 0.1) {                                                     \
    start = getTimeStamp();                                                    \
    for (int it = 0; it < iter; it++) {                                        \
      kernal;                                                                  \
    }                                                                          \
    end = getTimeStamp();                                                      \
    duration = end - start;                                                    \
    iter *= 2;                                                                 \
  }                                                                            \
  iter /= 2;                                                                   \
  std::cout << "Time taken : " << duration << "\n";                            \
  std::cout << "Performance : "                                                \
            << ((double)((double)iter * (double)NN) * mult_value) /            \
                   (duration * 1e9)                                            \
            << "GFlops/s \n";                                                  \
  std::cout << "The number of iterations " << iter                             \
            << " the size of the array " << N << "\n";

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

// #define Matrix_Check() \
//     // Check for basic matrix properties
// std::cout << "\n=== Matrix Diagnostics ===" << std::endl;

// // 1. Check if any diagonal is missing or zero
// int missing_diag = 0;
// int zero_diag = 0;
// int negative_diag = 0;

// for (int i = 0; i < A.rows; i++) {
//     bool found_diag = false;
//     double diag_val = 0.0;

//     for (int j = A.row_start[i]; j < A.row_start[i + 1]; j++) {
//         if (A.col_idx[j] == i) {
//             found_diag = true;
//             diag_val = A.values[j];
//             break;
//         }
//     }

//     if (!found_diag) {
//         missing_diag++;
//         if (missing_diag <= 5) {
//             std::cout << "Missing diagonal at row " << i << std::endl;
//         }
//     } else if (diag_val == 0.0) {
//         zero_diag++;
//     } else if (diag_val < 0.0) {
//         negative_diag++;
//         if (negative_diag <= 5) {
//             std::cout << "Negative diagonal at row " << i << ": " << diag_val
//             << std::endl;
//         }
//     }
// }

// std::cout << "Missing diagonals: " << missing_diag << std::endl;
// std::cout << "Zero diagonals: " << zero_diag << std::endl;
// std::cout << "Negative diagonals: " << negative_diag << std::endl;

// // 2. Check diagonal dominance
// double min_diag = 1e100;
// double max_diag = -1e100;
// int weak_rows = 0;

// for (int i = 0; i < A.rows; i++) {
//     double diag = 0.0;
//     double off_diag_sum = 0.0;

//     for (int j = A.row_start[i]; j < A.row_start[i + 1]; j++) {
//         if (A.col_idx[j] == i) {
//             diag = A.values[j];
//             min_diag = std::min(min_diag, diag);
//             max_diag = std::max(max_diag, diag);
//         } else {
//             off_diag_sum += std::abs(A.values[j]);
//         }
//     }

//     if (diag <= off_diag_sum) {
//         weak_rows++;
//         if (weak_rows <= 3) {
//             std::cout << "Row " << i << " not diagonally dominant: diag="
//                       << diag << ", off_diag_sum=" << off_diag_sum <<
//                       std::endl;
//         }
//     }
// }

// std::cout << "Diagonal range: [" << min_diag << ", " << max_diag << "]" <<
// std::endl; std::cout << "Weakly diagonally dominant rows: " << weak_rows <<
// "/" << A.rows << std::endl;

// // 3. Sample first row
// std::cout << "\nFirst row (row 0):" << std::endl;
// for (int j = A.row_start[0]; j < A.row_start[1] && j < A.row_start[0] + 10;
// j++) {
//     std::cout << "  col=" << A.col_idx[j] << ", val=" << A.values[j] <<
//     std::endl;
// }

// std::cout << "=========================\n" << std::endl;