#include "cg.hpp"
#include "gauss_seidel.hpp"
#include "jacobi.hpp"
#include "kernals.hpp"
#include "spmv.hpp"
#include "timing.hpp"
#include "utils.hpp"
#include "vec.hpp"
#include <iostream>
#include <omp.h>
#include <string>

int main(int argc, char *argv[]) {

  if (argc != 3) {
    std::cerr << "Usage " << argv[0] << " <kernal_name> \n";
    std::cerr << "Choose one of these kernals \n-> axpby <size>\n-> dot "
                 "<size> \n-> read_sparse <matrix_path>\n-> spmv "
                 "<matrix_path>\n-> jacobi "
                 "<matrix_path>\n-> jacobi_separate <matrix_path>\n-> "
                 "gauss_seidel <matrix_path>\n ";
    return 1;
  }

  std::string input_kernal = argv[1];
  int N = 1000, iter = 8, warmupIter = 8;
  std::string matrix_name = "matrices/matrix_band_small.mtx";
  if ((input_kernal == "axpby" || input_kernal == "dot")) {
    N = isdigit(*argv[2]) ? std::stoi(argv[2]) : 1000; // size of the aray
  } else if (input_kernal == "jacobi" || input_kernal == "gauss_seidel" ||
             input_kernal == "read_sparse" || input_kernal == "spmv" ||
             input_kernal == "cg" || input_kernal == "jacobi_separate") {
    matrix_name = argv[2];
  }

  int maxIter = 1000;

  if (input_kernal == "axpby") {
    std::cout << "in kernal size N : " << N << std::endl;

    PRINT_KERNAL("AXPBY");
    VecND a1 = create_vector<VecND>(0.1, N);
    VecND a2 = create_vector<VecND>(0.1, N);
    VecND a3 = create_vector<VecND>(0.1, N);
    const double mult_value = 3.0; // this is the number of flops
    HARNESS(axpby(a3, a2, a1, 1, 2, N), mult_value, N)
  }

  else if (input_kernal == "dot") {
    std::cout << "in kernal size N : " << N << std::endl;
    PRINT_KERNAL("DOT")
    VecND a1 = create_vector<VecND>(0.1, N);
    VecND a2 = create_vector<VecND>(0.2, N);
    double dot_result = 0.0;
    const double mult_value = 2.0; // is the number of flops
    HARNESS(dot(dot_result, a1, a2, N), mult_value, N)
  } else if (input_kernal == "read_sparse") {
    csr mat;
    read_matrix(&matrix_name[0], mat);
    calculate_b_c(mat);
    PRINT_SPARSE_DETAILS(mat);
  } else if (input_kernal == "spmv") {
    csr mat;
    read_matrix(&matrix_name[0], mat);
    // calculate_b_c(mat);
    // PRINT_SPARSE_DETAILS(mat);
    N = mat.rows;
    VecND rhs = create_vector<VecND>(0.1, N);
    VecND lhs = create_vector<VecND>(0.1, N);
    // modify this to reflect the actual Flops
    HARNESS(spmv(lhs, mat, rhs), 2, mat.nnz)
  } else if (input_kernal == "jacobi") {
    csr A;
    read_matrix(&matrix_name[0], A);
    // PRINT_SPARSE_DETAILS(A)
    N = A.rows;
    VecND b = create_vector<VecND>(2.0, N);
    VecND x_new = create_vector<VecND>(0.0, N);
    VecND x_old = create_vector<VecND>(1.0, N);
    TIME_SOLVER(jacobi(maxIter, A, b, x_new, x_old), maxIter, A)
  } else if (input_kernal == "jacobi_separate") {
    csr A;
    read_matrix(&matrix_name[0], A);
    // PRINT_SPARSE_DETAILS(A)
    N = A.rows;
    VecND D = create_vector<VecND>(1.0, N);
    VecND b = create_vector<VecND>(2.0, N);
    VecND x_new = create_vector<VecND>(0.0, N);
    VecND x_old = create_vector<VecND>(1.0, N);
    // get_diagonal(A, D, x_old, x_new);
    TIME_SOLVER(jacobi_separate(maxIter, A, b, x_new, x_old, D), maxIter, A)
  } else if (input_kernal == "gauss_seidel") {
    csr A;
    read_matrix(&matrix_name[0], A);
    N = A.rows;
    VecND b = create_vector<VecND>(2.0, N);
    VecND x = create_vector<VecND>(1.0, N);
    TIME_SOLVER(gauss_seidel(maxIter, A, b, x), maxIter, A)
  } else if (input_kernal == "cg") {
    csr A;
    std::cout << matrix_name << std::endl;
    read_matrix(&matrix_name[0], A);
    // PRINT_SPARSE_DETAILS(A)
    N = A.rows;
    VecND b = create_vector<VecND>(2.0, N);
    VecND x = create_vector<VecND>(1.0, N);
    TIME_SOLVER(cg_solver(maxIter, A, x, b), maxIter, A)
  } else if (input_kernal == "pcg") {
    csr A;
    std::cout << matrix_name << std::endl;
    read_matrix(&matrix_name[0], A);
  } else {
    std::cerr << "This kernal is not yet available \n";
  }
  return 0;
}
