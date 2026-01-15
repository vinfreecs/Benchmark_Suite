// 1. Compute r0 := b − Ax0, p0 := r0
// 2. For j = 0, 1, . . . , until convergence Do:
// 3. αj := (rj , Arj )/(Apj , Apj )
// 4. xj+1 := xj + αjpj
// 5. rj+1 := rj − αjApj
// 6. βj := (rj+1, Arj+1)/(rj , Arj )
// 7. pj+1 := rj+1 + βjpj
// 8. Compute Apj+1 = Arj+1 + βjApj
// 9. EndDo

#pragma once

#include "kernals.hpp"
#include "spmv.hpp"
#include "vec.hpp"
void cg_iteration(csr &A, csr &L, csr &U, VecND &A_D, VecND &A_D_inv,
                  VecND &L_D, VecND &U_D, VecND &x_new, VecND &x_old,
                  VecND &tmp, VecND &work, VecND &p_new, VecND &p_old,
                  VecND &r_new, VecND &r_old, VecND *z_new, VecND &z_old) {

  // TODO IMPLEMENT ALL THE FUNCTIONS
  // tmp <- A*p_old
  spmv(tmp, A, p_old);

  double tmp_dot;
  dot(tmp_dot, r_old, z_old, A.cols);

  // alpha <- (r_old, z_old) / (Ap_old, p_old)
  double alpha;
  dot(alpha, tmp, p_old, A.cols);
  alpha = tmp_dot / alpha;

  // x_new <- x_old + alpha * p_old
  sum_vectors(x_new, x_old, p_old, A.cols, alpha);

  // r_new <- r_old - alpha * Ap_old
  subtract_vectors(r_new, r_old, tmp, A.cols, alpha);

  //   TIapply_preconditioner(preconditioner, A->n_cols, L, U, A_D, A_D_inv,
  //   L_D,
  //                          U_D, z_new, r_new, tmp,
  //                          work SMAX_ARGS(0, smax, "M^{-1} * residual"));
  // beta <- (r_new, z_new) / (r_old, z_old)
  double beta;
  // TODO check this
  beta = dot(r_new, r_new, A.cols) / tmp_dot;
  // p_new <- z_new + beta * p_old
  sum_vectors(p_new, z_new, p_old, A.cols, beta)
}
void cg_solver() {}