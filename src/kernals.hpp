#pragma once
#include "vec.hpp"

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