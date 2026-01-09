#include "gauss_seidel.hpp"
#include "jacobi.hpp"
#include "kernals.hpp"
#include "spmv.hpp"
#include "timing.h"
#include "utils.hpp"
#include "vec.hpp"
#include <iostream>
#include <omp.h>
#include <string>

template <typename T> T create_vector(typename T::value_type value, int N) {
  T vec(N);
#pragma omp parallel for schedule(static)
  for (int i = 0; i < N; i++) {
    vec[i] = value;
  }
  return vec;
}

int main(int argc, char *argv[]) {

  // if (argc < 2) {
  //   std::cerr << "Usage " << argv[0] << " <kernal_name> <size> <iter>\n";
  //   std::cerr << "Choose one of these kernals \n-> axpby\n-> dot \n-> spmv
  //   \n"; return 1;
  // }

  std::string input_kernal = argv[1];
  int N = 1000, iter = 1000, warmupIter = 10;
  std::string matrix_name = "matrices/kkt_power.mtx";
  if (input_kernal == "axpby" || input_kernal == "dot" ||
      input_kernal == "spmv") {

    int N = isdigit(*argv[2]) ? std::stoi(argv[2]) : 1000; // size of the aray
    int iter =
        isdigit(*argv[3]) ? std::stod(argv[3]) : 1000; // number of iterations
    int warmupIter =
        (int)(iter / 5); // warmup iteration to avoid caching effects
  } else if (input_kernal == "jacobi" || input_kernal == "gauss_seidel") {
    matrix_name = argv[2];
  }

  if (input_kernal == "axpby") {
    PRINT_KERNAL("AXPBY");
    VecND a1 = create_vector<VecND>(0.1, N);
    VecND a2 = create_vector<VecND>(0.1, N);
    VecND a3 = create_vector<VecND>(0.1, N);
    const double mult_value = 3.0; // this is the number of flops
    HARNESS(axpby(a3, a2, a1, 1, 2, N), mult_value, N)
  }

  else if (input_kernal == "dot") {
    PRINT_KERNAL("DOT")
    VecND a1 = create_vector<VecND>(0.1, N);
    VecND a2 = create_vector<VecND>(0.2, N);
    double dot_result = 0.0;
    const double mult_value = 2.0; // is the number of flops
    HARNESS(dot(dot_result, a1, a2, N), mult_value, N)
  } else if (input_kernal == "spmv") {
    csr mat;
    read_matrix("matrices/lp_fffff800.mtx", mat);
    calculate_b_c(mat);
    PRINT_SPARSE_DETAILS(mat);
  } else if (input_kernal == "spmv_mult") {
    csr mat;
    read_matrix("matrices/nv1.mtx", mat);
    // calculate_b_c(mat);
    // PRINT_SPARSE_DETAILS(mat);
    N = mat.rows;
    VecND rhs = create_vector<VecND>(0.1, N);
    VecND lhs = create_vector<VecND>(0.1, N);
    // modify this to reflect the actual Flops
    HARNESS(spmv_vector_mult(mat, rhs, lhs), 1, mat.nnz)
  }
  // TODO verify if the residual calculation should be done after the
  // iterations or in the solver itself inside it is slow
  else if (input_kernal == "jacobi") {
    csr A;
    read_matrix("matrices/HPCG-25-25-25.mtx", A);
    N = A.rows;
    VecND b = create_vector<VecND>(1.0, N);
    VecND x_new = create_vector<VecND>(0.0, N);
    VecND x_old = create_vector<VecND>(0.1, N);
    int maxIter = 1000;
    TIME_SOLVER(jacobi(maxIter, A, b, x_new, x_old), maxIter)
  } else if (input_kernal == "gauss_seidel") {
    csr A;
    read_matrix("matrices/HPCG-25-25-25.mtx", A);
    N = A.rows;
    VecND b = create_vector<VecND>(1.0, N);
    VecND x = create_vector<VecND>(0.1, N);
    int maxIter = 1000;
    TIME_SOLVER(gauss_seidel(maxIter, A, b, x), maxIter)
  } else {
    std::cerr << "This kernal is not yet available \n";
  }
  return 0;
}
