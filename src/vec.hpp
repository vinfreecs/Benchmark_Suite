#pragma once
#include <memory>
#include <omp.h>
#include <vector>

// custom allocator to not initalize zero when using a vector
template <typename T> struct NoInitAllocator : public std::allocator<T> {
  using std::allocator<T>::allocator;

  template <class U> struct rebind {
    using other = NoInitAllocator<U>;
  };
  template <typename U, typename... Args>
  void construct(U *p, Args &&...args) {}
};

using VecND = std::vector<double, NoInitAllocator<double>>;
using VecNI = std::vector<int, NoInitAllocator<int>>;