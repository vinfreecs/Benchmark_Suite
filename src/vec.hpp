#pragma once
#include <iostream>

// 1D array class
// TODO add helper functions and other constructors
class vec {
public:
  vec(int dim) {
    dim_ = dim;
    data_ = new double[dim_];
#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < dim_; i++) {
      data_[i] = 0.1;
    }
  }

  ~vec() { delete[] data_; }

  // returns the length of the array
  int len() { return dim_; }

  double &operator()(size_t i) { return data_[i]; }
  const double &operator()(size_t i) const { return data_[i]; }

private:
  double *data_;
  int dim_;
};