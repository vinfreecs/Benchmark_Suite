#include "kernals.hpp"
#include "mmio.h"
#include "timing.h"
#include "vec.hpp"
#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <omp.h>
#include <string>
#include <vector>
#include "spmv.hpp"

#define PRINT_KERNAL(kernal)                                                   \
  std::cout << "-- BENCHMARKING " << kernal << " --" << std::endl;             \
  std::cout << "The number of iterations " << iter                             \
            << " the size of the array " << N << std::endl;

int main(int argc, char *argv[]) {

  if (argc < 4) {
    std::cerr << "Usage " << argv[0] << " <kernal_name> <size> <iter>"
              << std::endl;
    std::cerr << "Choose one of these kernals \n-> axpby\n-> dot " << std::endl;
    return 1;
  }

  int warmupIter = 80; // warmup iteration to avoid caching effects
  int N = isdigit(*argv[2]) ? std::stoi(argv[2]) : 1000; // size of the aray
  int iter =
      isdigit(*argv[3]) ? std::stod(argv[3]) : 1000; // number of iterations

  std::string input_kernal = argv[1];

  if (input_kernal == "axpby") {
    PRINT_KERNAL("AXPBY");
    auto a1 = new vec(N);
    auto a2 = new vec(N);
    auto a3 = new vec(N);

    HARNESS(axpby(a3, a2, a1, 1, 2, N))

    delete a1;
    delete a2;
    delete a3;
  } else if (input_kernal == "dot") {
    PRINT_KERNAL("DOT")
    auto a1 = new vec(N);
    auto a2 = new vec(N);
    double dot_result = 0.0;
    HARNESS(dot(dot_result, a1, a2, N))
    delete a1;
    delete a2;
  } else if (input_kernal == "spmv") {
    read_matrix("matrices/garon2.mtx");
  } else {
    std::cerr << "This kernal is not yet available" << std::endl;
  }
  return 0;
}
