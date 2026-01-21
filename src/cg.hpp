// 1. Compute r0 := b − Ax0, p0 := r0
// 2. For j = 0, 1, . . . , until convergence Do:
// 3. αj := (rj , rj )/(Apj , pj )
// 4. xj+1 := xj + αjpj
// 5. rj+1 := rj − αjApj
// 6. βj := (rj+1, rj+1)/(rj , rj )
// 7. pj+1 := rj+1 + βjpj
// 8. Compute Apj+1 = Arj+1 + βjApj
// 9. EndDo

#pragma once

#include "kernals.hpp"
#include "spmv.hpp"
#include "vec.hpp"
#include <utility>
void cg_iteration(csr &A, VecND &x_new, VecND &x_old, VecND &tmp, VecND &p_new,
                  VecND &p_old, VecND &r_new, VecND &r_old) {

  // tmp = A*p_old
  spmv(tmp, A, p_old);

  double tmp_dot = 0;
  dot(tmp_dot, r_old, r_old, A.cols);

  // alpha = (r_old, r_old) / (Ap_old, p_old)
  double alpha = 0;
  dot(alpha, tmp, p_old, A.cols);
  alpha = tmp_dot / alpha;

  // x_new = x_old + alpha * p_old
  sum_vectors(x_new, x_old, p_old, A.cols, alpha);

  // r_new = r_old - alpha * Ap_old
  subtract_vectors(r_new, r_old, tmp, A.cols, alpha);

  // beta = (r_new, r_new) / (r_old, r_old)
  double beta = 0;
  dot(beta, r_new, r_new, A.cols);
  beta = beta / tmp_dot;
  // p_new <- z_new + beta * p_old
  sum_vectors(p_new, r_new, p_old, A.cols, beta);
}
void cg_solver(int &maxIter, csr &A, VecND &x_old, VecND &b) {
  // initialisation
  double tol = 1e-4;
  int N = x_old.size();
  VecND x_new(N), p_new(N), p_old(N), r_new(N), r_old(N), tmp(N);
#pragma omp parallel for schedule(static)
  for (int i = 0; i < N; ++i) {
    tmp[i] = 0.0;
    x_new[i] = 0.0;
    p_new[i] = 0.0;
    p_old[i] = 0.0;
    r_new[i] = 0.0;
    r_old[i] = 0.0;
  }

#pragma omp parallel for schedule(static)
  for (int i = 0; i < A.rows; ++i) {
    double Ax_i = 0.0;
    for (int j = A.row_start[i]; j < A.row_start[i + 1]; ++j) {
      Ax_i += A.values[j] * x_old[A.col_idx[j]];
    }
    r_old[i] = b[i] - Ax_i;
    p_old[i] = r_old[i];
  }

  // iteration
  for (int iter = 0; iter < maxIter; iter++) {
    cg_iteration(A, x_new, x_old, tmp, p_new, p_old, r_new, r_old);
    // exchange
    std::swap(p_new, p_old);
    std::swap(x_new, x_old);
    std::swap(r_new, r_old);
    if (iter % 1 == 0) {
      double res = get_residual(A, b, x_old);
      std::cout << "Iter " << iter << ": Residual = " << res << std::endl;
      if (res < tol) {
        std::cout << "Solver : Conjugate Gradient Converged \n";
        maxIter = iter + 1;
        return;
      }
    }
  }
}