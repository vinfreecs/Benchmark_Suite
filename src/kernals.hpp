#pragma once

#define HARNESS(kernal)                \
  for (int i = 0; i < warmupIter; i++) \
  {                                    \
    kernal;                            \
  }                                    \
  double start, end;                   \
  start = getTimeStamp();              \
  for (int i = 0; i < iter; i++)       \
  {                                    \
    kernal;                            \
  }                                    \
  end = getTimeStamp();                \
  std::cout << "Time taken : " << end - start << std::endl;

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
void dot(double ans, vec *x, vec *y)
{

#pragma omp parallel for schedule(static)
  for (int i = 0; i < x->len(); i++)
  {
    ans += (*x)(i) * (*y)(i);
  }
}