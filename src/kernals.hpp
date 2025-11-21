#pragma once

#define HARNESS(kernal)                                     \
  for (int wit = 0; wit < warmupIter; wit++)                      \
  {                                                         \
    kernal;                                                 \
  }                                                         \
  double start, end;                                        \
  start = getTimeStamp();                                   \
  for (int it = 0; it < iter; it++)                            \
  {                                                         \
    kernal;                                                 \
  }                                                         \
  end = getTimeStamp();                                     \
  double duration = end - start;                            \
  std::cout << "Time taken : " << duration << std::endl;    \
  std::cout << "Performance : " << ((double)((double)iter * (double)N)) / (duration * 1e6) << "MIt/s" << std::endl;

// This is a axpby function which takes in 1D arrays and return an 1D array after computation
// C = Ax + By
void axpby(vec *ans, vec *x, vec *y, double a, double b, int N)
{
#pragma omp parallel for schedule(static)
  for (int i = 0; i < N; i++)
  {
    (*ans)(i) = a * (*x)(i) + b * (*y)(i);
  }
}
// This is a dot function which takes in two 1D arrays and return the dot product , ie, a double
void dot(double &ans, vec *x, vec *y)
{

#pragma omp parallel for schedule(static) reduction(+:ans)
  for (int i = 0; i < x->len(); i++)
  {
    ans += (*x)(i) * (*y)(i);
  }
}