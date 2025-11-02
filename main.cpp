#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <omp.h>
#include "include/grid.hpp"
#include "include/timing.hpp"
#include "include/kernals.hpp"



int main() {

  auto g1 = new grid(10000, 10000);
  auto g2 = new grid(10000, 10000);
  auto g3 = new grid(10000, 10000);

  START_TIMER(AXPBY);
  axpby(g3, g1, g2, 1, 2);
  STOP_TIMER(AXPBY);

  double dot_result = 0.0;
  START_TIMER(DOT);
  dot(dot_result, g1, g2);
  STOP_TIMER(DOT);

  delete g1;
  delete g2;
  delete g3;
}
