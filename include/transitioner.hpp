#pragma once

#include <chrono>

namespace sel {
class Transitioner {
public:
  Transitioner(float maxValue, int duration);

  float getValue();
  float targetValue();
  void setTargetValue(float value);
  void activate(float startValue);
  bool finished();
  double elapsed();

private:
  using clock_t = std::chrono::steady_clock;

  float startValue, endValue;
  int duration;
  std::chrono::time_point<clock_t> timeStart;
};

} // namespace sel
