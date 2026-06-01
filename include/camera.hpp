#pragma once

#include "transitioner.hpp"
#include <raylib.h>

namespace sel {
class Camera {
public:
  Camera();

  Camera3D getCamera();

  void Update();

private:
  Camera3D camera;
  float angle, dist, height;
  sel::Transitioner rotator;
};
} // namespace sel
