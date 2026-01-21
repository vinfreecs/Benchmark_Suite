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
#include <utility>

void pcg_iteration(std::string preconditioner = "jacobi", csr &A, csr &L,
                   csr &U, VecND &A_D, VecND &A_D_inv, VecND &L_D, VecND &U_D,
                   VecND &x_new, VecND &x_old, VecND &tmp, VecND &work,
                   VecND &p_new, VecND &p_old, VecND &r_new, VecND &r_old,
                   VecND &z_new, VecND &z_old) {}

void pcg_solver(int &maxIter, csr &A, VecND &x_old, VecND &b) {
  // initialisation
  double tol = 1e-4;
  int N = x_old.size();
  // TODO A_D, A_D_inv, L_D, U_D work,z_new,z_old, L,U initialisation
  VecND x_new(N), p_new(N), p_old(N), r_new(N), r_old(N), tmp(N), A_D(N),
      A_D_inv(N), L_D(N), U_D(N), z_new(N), z_old(N), work(N);
  csr L, U;
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

    pcg_iteration("jacobi", A, L, U, A_D, A_D_inv, L_D, U_D, x_new, x_old, tmp,
                  work, p_new, p_old, r_new, r_old, z_new, z_old);
    // // exchange
    // std::swap(p_new, p_old);
    // std::swap(x_new, x_old);
    // std::swap(r_new, r_old);
    // if (iter % 1 == 0) {
    //   double res = get_residual(A, b, x_old);
    //   std::cout << "Iter " << iter << ": Residual = " << res << std::endl;
    //   if (res < tol) {
    //     std::cout << "Solver : Conjugate Gradient Converged \n";
    //     maxIter = iter + 1;
    //     return;
    //   }
    // }
  }
}