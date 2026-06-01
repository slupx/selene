#include "functions.hpp"

#include <cmath>

float sel::easeInOutQuad(float x) {
  return x < 0.5 ? 2 * x * x : 1 - std::pow(-2 * x + 2, 2) / 2;
}