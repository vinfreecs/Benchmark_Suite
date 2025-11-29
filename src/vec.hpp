#pragma once
#include <omp.h>
#include <vector>

template <typename T> struct uninitialized {
  // FIX: Use empty braces, NOT = default.
  // This prevents the compiler from zero-initializing the memory.
  uninitialized() {}

  T val;

  constexpr operator T() const { return val; }

  // Accepting T by value is cleaner for simple types like double
  T operator=(T v) {
    val = v;
    return val;
  }
};

class vec {
public:
  vec(int dim) : dim_(dim) {
    // 1. Virtual Allocation (Fast)
    // Because uninitialized() {} does nothing, this only reserves address
    // space. The Master thread does NOT write to the array here.
    data_.resize(dim_);

    // 2. Physical Allocation (First Touch)
    // This loop triggers the actual Page Faults on the correct NUMA nodes.
#pragma omp parallel for schedule(static)
    for (int i = 0; i < dim_; i++) {
      data_[i] = 0.1;
    }
  }

  // Helper accessors
  double &operator[](int i) { return data_[i].val; }
  const double &operator[](int i) const { return data_[i].val; }
  int size() const { return dim_; }

private:
  std::vector<uninitialized<double>> data_;
  int dim_;
};