#pragma once
#include "kernals.hpp"
#include "spmv.hpp"
#include "vec.hpp"
#include <utility>

void jacobi_fused_iteration(const csr &A, const VecND &b, VecND &x_new,
                            const VecND &x_old) {
#pragma omp parallel for schedule(static)
  for (int row_idx = 0; row_idx < A.rows; ++row_idx) {
    double diag_elem = 1.0;
    double sum = 0.0;
    int start_row = A.row_start[row_idx];
    int stop_row = A.row_start[row_idx + 1];

    for (int nz_idx = start_row; nz_idx < stop_row; ++nz_idx) {
      if (row_idx == A.col_idx[nz_idx]) {
        diag_elem = A.values[nz_idx];
      } else {
        sum += A.values[nz_idx] * x_old[A.col_idx[nz_idx]];
      }
    }
    x_new[row_idx] = (b[row_idx] - sum) / diag_elem;
  }
}

void jacobi(int &maxIter, csr &A, VecND &b, VecND &x_new, VecND &x_old) {
  double tolerance = 0.0001;
  for (int k = 0; k < maxIter; k++) {
    jacobi_fused_iteration(A, b, x_new, x_old);
    if (k % 1 == 0) {
      double res = get_residual(A, b, x_new);
      std::cout << "Iter " << k << ": Residual = " << res << std::endl;
      if (res < tolerance) {
        std::cout << "Solver : Jacobi Converged \n";
        maxIter = k + 1;
        return;
      }
    }
    std::swap(x_old, x_new);
  }
}