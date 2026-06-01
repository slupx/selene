#include "transitioner.hpp"
#include <chrono>

#include "functions.hpp"

using namespace std::chrono;

sel::Transitioner::Transitioner(float maxValue, int duration) : duration(duration) {
  startValue = 0;
  endValue = 0;
}

float sel::Transitioner::getValue() {
  float delta = elapsed() / float(duration);

  if (delta >= 1) {
    return endValue;
  }
  return startValue + sel::easeInOutQuad(delta) * (endValue - startValue);
}

void sel::Transitioner::setTargetValue(float value) { endValue = value; }

float sel::Transitioner::targetValue() { return endValue; }

void sel::Transitioner::activate(float newValue) {
  startValue = finished() ? endValue : getValue();
  // startValue = endValue;
  endValue = newValue;
  timeStart = std::chrono::steady_clock::now();
}

bool sel::Transitioner::finished() { return elapsed() >= duration; }

double sel::Transitioner::elapsed() {
  return duration_cast<milliseconds>(clock_t::now() - timeStart).count();
}