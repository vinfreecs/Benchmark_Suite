#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <omp.h>
#include <string>
#include "timing.h"
#include "vec.hpp"
#include "kernals.hpp"

int main(int argc, char *argv[])
{

  if (argc != 2)
  {
    std::cout << argv[0] << " <kernal_name>" << std::endl;
    std::cout << "Choose one of these kernals \n-> axpby\n-> dot " << std::endl;
    return 1;
  }

  // add test runs
  int warmupIter = 10;
  int iter = 100;

  if (strcmp(argv[1], "axpby") == 0)
  {
    std::cout << "-- BENCHMARKING AXPBY --" << std::endl;
    auto a1 = new vec(10000);
    auto a2 = new vec(10000);
    auto a3 = new vec(10000);

    // A standard harness to time and can alter which kernal's benchmark to use at runtime
    HARNESS(axpby(a3, a2, a1, 1, 2, 10000))

    delete a1;
    delete a2;
    delete a3;
  }
  else if (strcmp(argv[1], "dot") == 0)
  {
    std::cout << "-- BENCHMARKING DOT --" << std::endl;
    auto a1 = new vec(10000);
    auto a2 = new vec(10000);
    double dot_result = 0.0;
    HARNESS(dot(dot_result, a1, a2))
    delete a1;
    delete a2;
  }
  else
  {
    std::cerr << "This is not available" << std::endl;
  }
  return 0;
}
