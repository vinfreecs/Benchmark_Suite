#pragma once
#include "spmv.hpp"
#include "vec.hpp"
#include <cmath>

// This is a axpby function which takes in 1D arrays and return an 1D array
// after computation C = aX + bY
void axpby(VecND &ans, const VecND &vec1, const VecND &vec2, double a, double b,
           int N) {

#pragma omp parallel for schedule(static)
  for (int i = 0; i < N; i++) {
    ans[i] = (a * vec1[i]) + (b * vec2[i]);
  }
}

// This is a dot function which takes in two 1D arrays and return the dot
// product , ie, a double
void dot(double &ans, VecND &vec1, VecND &vec2, int N) {
  double temp_sum = 0.0;
#pragma omp parallel for schedule(static) reduction(+ : temp_sum)
  for (int i = 0; i < N; i++) {
    temp_sum += vec1[i] * vec2[i];
  }
  ans = temp_sum;
}

void sum_vectors(VecND &result_vec, const VecND &vec1, const VecND &vec2, int N,
                 double scale = 1.0) {
#pragma omp parallel for schedule(static)
  for (int i = 0; i < N; ++i) {
    result_vec[i] = vec1[i] + (scale * vec2[i]);
  }
}

void subtract_vectors(VecND &result_vec, const VecND &vec1, const VecND &vec2,
                      int N, double scale = 1.0) {
#pragma omp parallel for schedule(static)
  for (int i = 0; i < N; ++i) {
    result_vec[i] = vec1[i] - (scale * vec2[i]);
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

void get_diagonal(csr &A, VecND &D) {
#pragma omp parallel for schedule(static)
  for (int row_idx = 0; row_idx < A.rows; ++row_idx) {
    double diag_elem = 1.0;
    int start_row = A.row_start[row_idx];
    int stop_row = A.row_start[row_idx + 1];

    for (int nz_idx = start_row; nz_idx < stop_row; ++nz_idx) {
      if (row_idx == A.col_idx[nz_idx]) {
        diag_elem = A.values[nz_idx];
        break;
      }
    }
    D[row_idx] = diag_elem;
  }
}

void elemwise_div_vectors(VecND &result_vec, VecND &vec1, VecND &vec2, int N,
                          double scale = 1.0) {
#pragma omp parallel for schedule(static)
  for (int i = 0; i < N; ++i) {
    result_vec[i] = vec1[i] / (scale * vec2[i]);
  }
}

void sptrsv(csr &L, VecND &x, VecND &D, VecND &b) {
  double sum;
  for (int row = 0; row < L.rows; ++row) {
    sum = 0.0;
    int row_start = L.row_start[row];
    int row_stop = L.row_start[row + 1];

    for (int nz_idx = row_start; nz_idx < row_stop; ++nz_idx) {
      sum += L.values[nz_idx] * x[L.col_idx[nz_idx]];
    }

    x[row] = (b[row] - sum) / D[row];
  }
}

void bsptrsv(csr &U, VecND &x, VecND &D, VecND &b) {

  for (int row = U.rows - 1; row >= 0; --row) {
    double sum = 0.0;
    int row_start = U.row_start[row];
    int row_stop = U.row_start[row + 1];

    for (int nz_idx = row_start; nz_idx < row_stop; ++nz_idx) {
      sum += U.values[nz_idx] * x[U.col_idx[nz_idx]];
    }

    x[row] = (b[row] - sum) / D[row];
  }
}

void elemwise_mult_vectors(VecND &result_vec, VecND &vec1, VecND &vec2, int N) {
#pragma omp parallel for schedule(static)
  for (int i = 0; i < N; ++i) {
    result_vec[i] = vec1[i] * vec2[i];
  }
}

void apply_preconditioner(std::string preconditioner, const int N,
                          csr &L_strict, csr &U_strict, VecND &A_D,
                          VecND &output, VecND &input) {

  if (preconditioner == "jacobi") {
    elemwise_div_vectors(output, input, A_D, N);
  } else if (preconditioner == "gs") {
    sptrsv(L_strict, output, A_D, input);
  } else if (preconditioner == "sgs") {
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; ++i) {
      output[i] = input[i];
    }
    sptrsv(L_strict, output, A_D, output);
    elemwise_mult_vectors(output, output, A_D, N);
    bsptrsv(U_strict, output, A_D, output);
  }
  // TODO for else
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
}
