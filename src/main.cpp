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

  int warmupIter = 80; // warmup iteration to avoid caching effects
  int N = isdigit(*argv[2]) ? std::stoi(argv[2]) : 1000; // size of the aray
  int iter =
      isdigit(*argv[3]) ? std::stod(argv[3]) : 1000; // number of iterations
  std::string input_kernal = argv[1];

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
      a2[i] = 0.1;
    }
    double dot_result = 0.0;
    const double mult_value = 2.0; // is the number of flops
    HARNESS(dot(dot_result, a1, a2, N), mult_value, N)
  } else if (input_kernal == "spmv") {
    csr mat;
    read_matrix("matrices/garon2.mtx", mat);
    std::cout << "Rows : " << mat.rows << " Cols : " << mat.cols
              << " nnz : " << mat.nnz << "\n";
    std::cout << "number of elements in values : " << mat.values.size()
              << std::endl;
  } else if (input_kernal == "spmv_mult") {
    csr mat;
    read_matrix("matrices/garon2.mtx", mat);
    VecND rhs(mat.rows);
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
  } else {
    std::cerr << "This kernal is not yet available \n";
  }
  return 0;
}
