// 1. Compute r0 := b − Ax0, z0 = M−1
// r0, and p0 := z0
// 2. For j = 0, 1, . . ., until convergence Do:
// 3. αj := (rj , zj )/(Apj , pj )
// 4. xj+1 := xj + αjpj
// 5. rj+1 := rj − αjApj
// 6. zj+1 := M−1*rj+1
// 7. βj := (rj+1, zj+1)/(rj , zj )
// 8. pj+1 := zj+1 + βjpj
// 9. EndDo

#pragma once

#include "kernals.hpp"
#include "spmv.hpp"
#include "vec.hpp"

void pcg_iteration(csr &A, csr &L, csr &U, VecND &A_D, VecND &x_new,
                   VecND &x_old, VecND &tmp, VecND &work, VecND &p_new,
                   VecND &p_old, VecND &r_new, VecND &r_old, VecND &z_new,
                   VecND &z_old, std::string preconditioner = "jacobi") {

  spmv(tmp, A, p_old);

  double tmp_dot = 0;
  dot(tmp_dot, r_old, z_old, A.cols);

  // alpha <- (r_old, z_old) / (Ap_old, p_old)
  double alpha = 0;
  dot(alpha, tmp, p_old, A.cols);
  alpha = tmp_dot / alpha;

  // x_new <- x_old + alpha * p_old
  sum_vectors(x_new, x_old, p_old, A.cols, alpha);

  // r_new <- r_old - alpha * Ap_old
  subtract_vectors(r_new, r_old, tmp, A.cols, alpha);

  // z_new <- M^{-1}r_new
  apply_preconditioner(preconditioner, A.cols, L, U, A_D, z_new, r_new);

  // beta <- (r_new, z_new) / (r_old, z_old)
  double beta = 0;
  dot(beta, r_new, z_new, A.cols);
  beta = beta / tmp_dot;

  // p_new <- z_new + beta * p_old
  sum_vectors(p_new, z_new, p_old, A.cols, beta);
}

void pcg_solver(int &maxIter, csr &A, VecND &x_old, VecND &b,
                std::string preconditioner = "jacobi") {

  // initialisation
  double tol = 1e-4;
  int N = x_old.size();
  VecND x_new(N), p_new(N), p_old(N), r_new(N), r_old(N), tmp(N), A_D(N),
      z_new(N), z_old(N), work(N);
  csr L, U;
  split_matrix(A, L, A_D, U);
  // get_diagonal(A, A_D);

#pragma omp parallel for schedule(static)
  for (int i = 0; i < N; ++i) {
    tmp[i] = 0.0;
    x_new[i] = 0.0;
    p_new[i] = 0.0;
    p_old[i] = 0.0;
    r_new[i] = 0.0;
    r_old[i] = 0.0;
    x_new[i] = 0.0;
    z_new[i] = 0.0;
    z_old[i] = 0.0;
  }

#pragma omp parallel for schedule(static)
  for (int i = 0; i < A.rows; ++i) {
    double Ax_i = 0.0;
    for (int j = A.row_start[i]; j < A.row_start[i + 1]; ++j) {
      Ax_i += A.values[j] * x_old[A.col_idx[j]];
    }
    r_old[i] = b[i] - Ax_i;
  }
  apply_preconditioner(preconditioner, N, L, U, A_D, z_old, r_old);

#pragma omp parallel for schedule(static)
  for (int i = 0; i < N; i++) {
    p_old[i] = z_old[i];
  }

  // iteration
  for (int iter = 0; iter < maxIter; iter++) {

    pcg_iteration(A, L, U, A_D, x_new, x_old, tmp, work, p_new, p_old, r_new,
                  r_old, z_new, z_old, preconditioner);
    // exchange
    std::swap(p_new, p_old);
    std::swap(x_new, x_old);
    std::swap(r_new, r_old);
    std::swap(z_old, z_new);
    if (iter % 1 == 0) {
      double res = get_residual(A, b, x_old);
      std::cout << "Iter " << iter << ": Residual = " << res << std::endl;
      if (res < tol) {
        std::cout << "Solver : Preconditioned Conjugate Gradient Converged \n";
        maxIter = iter + 1;
        return;
      }
    }
  }
}