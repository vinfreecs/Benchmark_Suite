#pragma once
#include "jacobi.hpp"
#include "spmv.hpp"
#include "vec.hpp"
void gs_fused_iteration(csr &A, VecND &b, VecND &x) {
  double diag_elem = 1.0;

  for (int row_idx = 0; row_idx < A.rows; ++row_idx) {
    double sum = 0.0;
    int start_row = A.row_start[row_idx];
    int stop_row = A.row_start[row_idx + 1];

    for (int nz_idx = start_row; nz_idx < stop_row; ++nz_idx) {
      if (row_idx == A.col_idx[nz_idx]) {
        diag_elem = A.col_idx[nz_idx];
      } else {
        sum += A.values[nz_idx] * x[A.col_idx[nz_idx]];
      }
    }
    x[row_idx] = (b[row_idx] - sum) / diag_elem;
  }
}

void gauss_seidel(int maxIter, csr &A, VecND &b, VecND &x) {
  double tolerance = 0.0001;
  for (int iter = 0; iter < maxIter; iter++) {
    VecND x_old = x;
    gs_fused_iteration(A, b, x);
    double error = err_norm(x, x_old);
    if (error < tolerance) {
      return;
    }
  }
}