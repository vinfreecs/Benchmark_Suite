#pragma once
#include "vec.hpp"
#include <vector>

#define HARNESS(kernal, mult_value)                                            \
  for (int wit = 0; wit < warmupIter; wit++) {                                 \
    kernal;                                                                    \
  }                                                                            \
  double start, end;                                                           \
  start = getTimeStamp();                                                      \
  for (int it = 0; it < iter; it++) {                                          \
    kernal;                                                                    \
  }                                                                            \
  end = getTimeStamp();                                                        \
  double duration = end - start;                                               \
  std::cout << "Time taken : " << duration << std::endl;                       \
  std::cout << "Performance : "                                                \
            << ((double)((double)iter * (double)N) * mult_value) /             \
                   (duration * 1e9)                                            \
            << "GFlops/s" << std::endl;

// This is a axpby function which takes in 1D arrays and return an 1D array
// after computation C = Ax + By
void axpby(Vec &ans, const Vec &x, const Vec &y, double a, double b, int N) {

#pragma omp parallel for schedule(static)
  for (int i = 0; i < N; i++) {
    ans[i] = (a * x[i]) + (b * y[i]);
  }
}

// This is a dot function which takes in two 1D arrays and return the dot
// product , ie, a double
void dot(double &ans, Vec &x, Vec &y, int N) {
  double temp_sum = 0.0;
#pragma omp parallel for schedule(static) reduction(+ : temp_sum)
  for (int i = 0; i < N; i++) {
    temp_sum += x[i] * y[i];
  }
  ans = temp_sum;
}