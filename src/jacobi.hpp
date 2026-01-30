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
      // std::cout << "Iter " << k << ": Residual = " << res << std::endl;
      if (res < tolerance) {
        std::cout << "Iter " << k << ": Residual = " << res << std::endl;
        std::cout << "Solver : Jacobi Converged \n";
        maxIter = k + 1;
        return;
      }
    }
    std::swap(x_old, x_new);
  }
}

void normalize_x(VecND &x_new, VecND &x_old, VecND &D, VecND &b, int rows) {
#pragma omp parallel for schedule(static)
  for (int row_idx = 0; row_idx < rows; ++row_idx) {
    double scaled_x_old = D[row_idx] * x_old[row_idx];

    double adjusted_x = x_new[row_idx] - scaled_x_old;

    x_new[row_idx] = (b[row_idx] - adjusted_x) / D[row_idx];
  }
}

void jacobi_separate_iteration(csr &A, VecND &D, VecND &b, VecND &x_new,
                               VecND &x_old) {

  // x_new <- A*x_old
  spmv(x_new, A, x_old);

  // x_new <- D^{-1}(b - (x_new - D))
  normalize_x(x_new, x_old, D, b, A.rows);
}

void jacobi_separate(int &maxIter, csr &A, VecND &b, VecND &x_new, VecND &x_old,
                     VecND &D) {
  double tolerance = 0.0001;
  get_diagonal(A, D);
  for (int k = 0; k < maxIter; k++) {
    jacobi_separate_iteration(A, D, b, x_new, x_old);
    if (k % 1 == 0) {
      double res = get_residual(A, b, x_new);
      // std::cout << "Iter " << k << ": Residual = " << res << std::endl;
      if (res < tolerance) {
        std::cout << "Iter " << k << ": Residual = " << res << std::endl;
        std::cout << "Solver : Jacobi Separate Converged \n";
        maxIter = k + 1;
        return;
      }
    }
    std::swap(x_old, x_new);
  }
}

void spmv_orphaned_call(VecND &lhs, csr &smat, VecND &rhs) {
#pragma omp for schedule(static)
  for (int i = 0; i < smat.rows; i++) {
    double sum = 0.0;
    int start = smat.row_start[i];
    int end = smat.row_start[i + 1];
#pragma omp simd reduction(+ : sum)
    for (int j = start; j < end; j++) {
      sum += smat.values[j] * rhs[smat.col_idx[j]];
    }
    lhs[i] = sum;
  }
}

void normalize_x_orphaned_call(VecND &x_new, VecND &x_old, VecND &D, VecND &b,
                               int rows) {
#pragma omp for schedule(static)
  for (int row_idx = 0; row_idx < rows; ++row_idx) {
    double scaled_x_old = D[row_idx] * x_old[row_idx];

    double adjusted_x = x_new[row_idx] - scaled_x_old;

    x_new[row_idx] = (b[row_idx] - adjusted_x) / D[row_idx];
  }
}

void jacobi_orphaned_iteration(csr &A, VecND &D, VecND &b, VecND &x_new,
                               VecND &x_old) {

#pragma omp parallel
  {
    spmv_orphaned_call(x_new, A, x_old);
    normalize_x_orphaned_call(x_new, x_old, D, b, A.rows);
  }
}

void jacobi_orphaned(int &maxIter, csr &A, VecND &b, VecND &x_new, VecND &x_old,
                     VecND &D) {

  double tolerance = 0.0001;
  get_diagonal(A, D);
  for (int k = 0; k < maxIter; k++) {
    jacobi_orphaned_iteration(A, D, b, x_new, x_old);
    if (k % 1 == 0) {
      double res = get_residual(A, b, x_new);
      // std::cout << "Iter " << k << ": Residual = " << res << std::endl;
      if (res < tolerance) {
        std::cout << "Iter " << k << ": Residual = " << res << std::endl;
        std::cout << "Solver : Jacobi Orphaned Converged \n";
        maxIter = k + 1;
        return;
      }
    }
    std::swap(x_old, x_new);
  }
}