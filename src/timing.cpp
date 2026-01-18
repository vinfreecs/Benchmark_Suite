#include "timing.hpp"
#include <chrono>

double getTimeStamp() {
  auto now = std::chrono::steady_clock::now();
  auto duration = now.time_since_epoch();
  std::chrono::duration<double> seconds = duration;

  return seconds.count();
}
