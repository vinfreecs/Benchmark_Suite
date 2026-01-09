#pragma once
#include "spmv.hpp"
#include "vec.hpp"
#include <cmath>
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

double get_residual(const csr &A, const VecND &b, const VecND &x) {
  double sum_sq_diff = 0.0;

#pragma omp parallel for schedule(static) reduction(+ : sum_sq_diff)
  for (int i = 0; i < A.rows; ++i) {
    double Ax_i = 0.0;
    for (int j = A.row_start[i]; j < A.row_start[i + 1]; ++j) {
      Ax_i += A.values[j] * x[A.col_idx[j]];
    }

    double r_i = b[i] - Ax_i;
    sum_sq_diff += r_i * r_i;
  }

  return std::sqrt(sum_sq_diff);
}

void jacobi(int maxIter, csr &A, VecND &b, VecND &x_new, VecND &x_old) {
  double tolerance = 0.001;
  for (int k = 0; k < maxIter; k++) {
    jacobi_fused_iteration(A, b, x_new, x_old);
    // TODO omplement norm
    if (k % 1 == 0) {
      double res = get_residual(A, b, x_new);
      std::cout << "Iter " << k << ": Residual = " << res << std::endl;
      if (res < tolerance) {
        std::cout << "Solver : Jacobi Converged \n";
        return;
      }
    }
    // TODO will not work redo
    std::swap(x_old, x_new);
  }
}