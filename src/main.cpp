#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <omp.h>
#include "timing.h"
#include "vec.hpp"
#include "kernals.hpp"

int main()
{

  auto a1 = new vec(10000);
  auto a2 = new vec(10000);
  auto a3 = new vec(10000);

  // add test runs
  int warmupIter = 10;
  int iter = 100;

  // A standard harness to time and can alter which kernal's benchmark to use at runtime
  HARNESS(axpby(a3, a2, a1, 1, 2, 10000))

  double dot_result = 0.0;

  // HARNESS(dot(dot_result, a1, a2))

  delete a1;
  delete a2;
  delete a3;
}
