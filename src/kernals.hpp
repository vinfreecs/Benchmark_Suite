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