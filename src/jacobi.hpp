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

double err_norm(const VecND &x_new, const VecND &x_old) {
  double ans = 0;
  size_t n = x_new.size();
#pragma omp parallel for schedule(static) reduction(+ : ans)
  for (int i = 0; i < n; i++) {
    double diff = x_new[i] - x_old[i];
    ans += diff * diff;
  }
  return std::sqrt(ans);
}

void jacobi(int maxIter, csr &A, VecND &b, VecND &x_new, VecND &x_old) {
  double tolerance = 0.001;
  for (int k = 0; k < maxIter; k++) {
    jacobi_fused_iteration(A, b, x_new, x_old);
    // TODO omplement norm
    double error = err_norm(x_old, x_new);
    if (error < tolerance) {
      return;
    }
    // TODO will not work redo
    std::swap(x_old, x_new);
  }
}