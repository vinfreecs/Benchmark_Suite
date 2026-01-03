#include "spmv.hpp"
#include "vec.hpp"
#include <utility>

void jacobi_fused_iteration(const crs &A, const VecND &b, const VecND &x_new,
                            const VecND &x_old) {
  double diag_elem;

#pragma omp parallel for schedule(static)
  for (int row_idx = 0; row_idx < A.rows; ++row_idx) {
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
#pragma omp parallel for schedule(static) reduction(+ : ans)
  for (int i = 0; i < x_new.size(); i++) {
    ans += x_new[i] - x_old[i];
  }
  return ans;
}

void jacobi(int maxIter, const crs &A, const VecND &b, const VecND &x_new,
            const VecND &x_old) {
  for (int k = 0; k < maxIter; k++) {
    jacobi_fused_iteration(A, b, x_new, x_old);
    // TODO omplement norm
    double error = err_norm(x_old, x_new);
    double tolerance = 0.001;
    if (error < tolerance) {
      return;
    }
    // TODO will not work redo
    std::swap(x_old, x_new);
  }
}