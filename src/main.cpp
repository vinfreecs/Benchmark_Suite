#include "kernals.hpp"
#include "timing.h"
#include "vec.hpp"
#include <iostream>
#include <omp.h>
#include <string>

#define PRINT_KERNAL(kernal)                                                   \
  std::cout << "-- BENCHMARKING " << kernal << " --" << std::endl;             \
  std::cout << "The number of iterations " << iter                             \
            << " the size of the array " << N << std::endl;

int main(int argc, char *argv[]) {

  if (argc < 4) {
    std::cerr << "Usage " << argv[0] << " <kernal_name> <size> <iter> \n";
    std::cerr << "Choose one of these kernals \n-> axpby\n-> dot \n";
    return 1;
  }

  int warmupIter = 80; // warmup iteration to avoid caching effects
  int N = isdigit(*argv[2]) ? std::stoi(argv[2]) : 1000; // size of the aray
  int iter =
      isdigit(*argv[3]) ? std::stod(argv[3]) : 1000; // number of iterations

  std::string input_kernal = argv[1];

  if (input_kernal == "axpby") {
    PRINT_KERNAL("AXPBY");
    std::vector<uninitialized<double>> a1(N);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
      a1[i] = 0.1;
    }
    std::vector<uninitialized<double>> a2(N);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
      a2[i] = 0.1;
    }
    std::vector<uninitialized<double>> a3(N);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
      a3[i] = 0.1;
    }
    HARNESS(axpby(a3, a2, a1, 1, 2, N))
  } else if (input_kernal == "dot") {
    PRINT_KERNAL("DOT")
    std::vector<uninitialized<double>> a1(N);
    std::vector<uninitialized<double>> a2(N);
    double dot_result = 0.0;
    HARNESS(dot(dot_result, a1, a2, N))
  } else {
    std::cerr << "This kernal is not yet available" << std::endl;
  }
  return 0;
}
