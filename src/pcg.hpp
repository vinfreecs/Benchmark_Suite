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

  std::cout << "Entered pcg_iteration \n";

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
  std::cout << "Exit pcg_iteration \n";
}

void split_matrix(csr &A, csr &L, VecND &D, csr &U) {
  int N = A.rows;
  if (D.size() != N) {
    std::cerr << "Error: Vector dimension mismatch in split_matrix!"
              << std::endl;
    std::cerr << "Matrix rows: " << N << " vs Vector size: " << D.size()
              << std::endl;
    exit(1);
    // Or if VecND supports resizing: D.resize(N);
  }
  std::vector<double> l_vals, u_vals;
  std::vector<int> l_cols, u_cols;
  std::vector<int> l_row_ptr(N + 1, 0), u_row_ptr(N + 1, 0);
  std::cout << "Entered split_matrix \n";
  for (int row = 0; row < N; row++) {

    int start = A.row_start[row];
    int stop = A.row_start[row + 1];

    int l_count = 0;
    int u_count = 0;
    for (int i = start; i < stop; i++) {
      int col = A.col_idx[i];
      double val = A.values[i];
      if (row == col) {
        D[row] = val;
      } else if (col < row) {
        l_vals.push_back(val);
        l_cols.push_back(col);
        l_count++;
      } else {
        u_vals.push_back(val);
        u_cols.push_back(col);
        u_count++;
      }
    }
    l_row_ptr[row + 1] = l_row_ptr[row] + l_count;
    u_row_ptr[row + 1] = u_row_ptr[row] + u_count;
  }

  L.rows = N;
  L.cols = N;
  L.nnz = l_vals.size();
  U.rows = N;
  U.cols = N;
  U.nnz = u_vals.size();

  L.col_idx.resize(L.nnz);
  L.values.resize(L.nnz);
  L.row_start.resize(L.rows + 1);

  U.col_idx.resize(U.nnz);
  U.values.resize(U.nnz);
  U.row_start.resize(U.rows + 1);

#pragma omp parallel for schedule(static)
  for (int i = 0; i < u_vals.size(); i++) {
    U.values[i] = u_vals[i];
    U.col_idx[i] = u_cols[i];
  }
#pragma omp parallel for schedule(static)
  for (int i = 0; i < l_vals.size(); i++) {
    L.values[i] = l_vals[i];
    L.col_idx[i] = l_cols[i];
  }
#pragma omp parallel for schedule(static)
  for (int i = 0; i <= N; i++) {
    L.row_start[i] = l_row_ptr[i];
  }
#pragma omp parallel for schedule(static)
  for (int i = 0; i <= N; i++) {
    U.row_start[i] = u_row_ptr[i];
  }
  std::cout << "exit split_matrix \n";
}

void pcg_solver(int &maxIter, csr &A, VecND &x_old, VecND &b) {
  std::cout << "Entered pcg_solver \n";

  // initialisation
  double tol = 1e-4;
  int N = x_old.size();
  VecND x_new(N), p_new(N), p_old(N), r_new(N), r_old(N), tmp(N), A_D(N),
      z_new(N), z_old(N), work(N);
  csr L, U;
  split_matrix(A, L, A_D, U);
  // get_diagonal(A, A_D);

  std::cout << "Entered Intialisation 1 \n";

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

  std::cout << "Exit Intialisation 1 \n";

  std::cout << "Entered Intialisation 2 \n";

#pragma omp parallel for schedule(static)
  for (int i = 0; i < A.rows; ++i) {
    double Ax_i = 0.0;
    for (int j = A.row_start[i]; j < A.row_start[i + 1]; ++j) {
      Ax_i += A.values[j] * x_old[A.col_idx[j]];
    }
    r_old[i] = b[i] - Ax_i;
  }
  std::cout << "Exit Intialisation 2 \n";

  std::cout << "Entered apply_preconditioner \n";

  apply_preconditioner("jacobi", N, L, U, A_D, z_old, r_old);
  std::cout << "Exit apply_preconditioner \n";

#pragma omp parallel for schedule(static)
  for (int i = 0; i < N; i++) {
    p_old[i] = z_old[i];
  }

  // iteration
  for (int iter = 0; iter < maxIter; iter++) {

    std::string preconditioner = "sgs";
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
  std::cout << "exit pcg_solver \n";
}