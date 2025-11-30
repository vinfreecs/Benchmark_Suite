#pragma once
#include "vec.hpp"

#define HARNESS(kernal)                                                        \
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
            << ((double)((double)iter * (double)N)) / (duration * 1e6)         \
            << "MIt/s" << std::endl;

// This is a axpby function which takes in 1D arrays and return an 1D array
// after computation C = Ax + By
void axpby(std::vector<uninitialized<double>> &ans,
           const std::vector<uninitialized<double>> &x,
           const std::vector<uninitialized<double>> &y, double a, double b,
           int N) {

// Ensure the loop is large enough to benefit from threads
#pragma omp parallel for schedule(static)
  for (int i = 0; i < N; i++) {
    ans[i] = (a * x[i]) + (b * y[i]);
  }
}
// This is a dot function which takes in two 1D arrays and return the dot
// product , ie, a double
void dot(double &ans, const std::vector<uninitialized<double>> &x,
         const std::vector<uninitialized<double>> &y, int N) {

  double temp_sum = 0.0;

// Use OpenMP reduction to sum safely in parallel
#pragma omp parallel for schedule(static) reduction(+ : temp_sum)
  for (int i = 0; i < N; i++) {
    temp_sum += x[i] * y[i];
  }
  ans = temp_sum;
}