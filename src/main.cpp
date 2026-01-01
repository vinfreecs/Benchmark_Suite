#include "kernals.hpp"
#include "spmv.hpp"
#include "timing.h"
#include "utils.hpp"
#include "vec.hpp"
#include <iostream>
#include <omp.h>
#include <string>

int main(int argc, char *argv[]) {

  if (argc < 4) {
    std::cerr << "Usage " << argv[0] << " <kernal_name> <size> <iter>\n";
    std::cerr << "Choose one of these kernals \n-> axpby\n-> dot \n-> spmv \n";
    return 1;
  }

  int N = isdigit(*argv[2]) ? std::stoi(argv[2]) : 1000; // size of the aray
  int iter =
      isdigit(*argv[3]) ? std::stod(argv[3]) : 1000; // number of iterations
  std::string input_kernal = argv[1];
  int warmupIter = (int)(iter / 5); // warmup iteration to avoid caching effects

  if (input_kernal == "axpby") {
    PRINT_KERNAL("AXPBY");
    VecND a1(N);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
      a1[i] = 0.1;
    }
    VecND a2(N);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
      a2[i] = 0.1;
    }
    VecND a3(N);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
      a3[i] = 0.1;
    }
    const double mult_value = 3.0; // this is the number of flops
    HARNESS(axpby(a3, a2, a1, 1, 2, N), mult_value, N)
  }

  else if (input_kernal == "dot") {
    PRINT_KERNAL("DOT")
    VecND a1(N);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
      a1[i] = 0.1;
    }
    VecND a2(N);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
      a2[i] = 0.2;
    }
    double dot_result = 0.0;
    const double mult_value = 2.0; // is the number of flops
    HARNESS(dot(dot_result, a1, a2, N), mult_value, N)
  } else if (input_kernal == "spmv") {
    csr mat;
    read_matrix("matrices/kkt_power.mtx", mat);
    // std::cout << "\n row_start values : \n";
    // for (int i = 0; i < 20; i++) {
    //   std::cout << " " << mat.row_start[i] << " ";
    // }
    // std::cout << "\n col_idx values : \n";
    // for (int i = 0; i < 20; i++) {
    //   std::cout << " " << mat.col_idx[i] << " ";
    // }
    PRINT_SPARSE_DETAILS(mat);
  } else if (input_kernal == "spmv_mult") {
    csr mat;
    read_matrix("matrices/kkt_power.mtx", mat);
    // PRINT_SPARSE_DETAILS(mat);
    VecND rhs(mat.rows);
    N = mat.rows; // we want all the values in the array to some value TODO
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
      rhs[i] = 0.1;
    }
    VecND lhs(mat.rows);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
      lhs[i] = 0.1;
    }
    // modify this to reflect the actual Flops
    HARNESS(spmv_vector_mult(mat, rhs, lhs), 1, mat.nnz)
    calculate_b_c(mat);
  } else if (input_kernal == "jacobi") {
    csr A;
    read_matrix("matrices/kkt_power.mtx", A);
    VecND b(A.rows);
    N = A.rows;
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
      b[i] = 0.1;
    }
    VecND x(A.rows);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
      x[i] = 0.0;
    }
    int maxIter = 10000;
    jacobi(maxIter, &A, b, x_new, x_old);
  } else {
    std::cerr << "This kernal is not yet available \n";
  }
  return 0;
}
