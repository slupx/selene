#include "camera.hpp"
#include <cmath>
#include <raylib.h>

sel::Camera::Camera() : camera{{0.0}}, angle{0.0}, dist{20.0}, height{10.0}, rotator(360, 500) {
  camera.position = {0, 3, 3};
  camera.up = (Vector3){0.0, 1.0, 0.0};
  camera.fovy = 45;
  camera.target = (Vector3){0.0, 1.0, 0.0};
  camera.projection = CAMERA_ORTHOGRAPHIC;
  camera.fovy = 20.0f;
}

Camera3D sel::Camera::getCamera() { return camera; }

void sel::Camera::Update() {
  float mouseWheelMovement = GetMouseWheelMove();

  camera.fovy = fmax(mouseWheelMovement + camera.fovy, 10.0);

  // clip angle to 360 not breaking animations
  if (rotator.targetValue() == angle) {
    angle = int(angle) % 360;
    rotator.setTargetValue(angle);
  }
  if (IsKeyPressed(KEY_LEFT)) {
    rotator.activate(rotator.targetValue() + 45);
  }
  if (IsKeyPressed(KEY_RIGHT)) {
    rotator.activate(rotator.targetValue() - 45);
  }

  if (IsKeyDown(KEY_UP)) {
    height += 0.2;
  }
  if (IsKeyDown(KEY_DOWN)) {
    height -= 0.2;
  }

  angle = rotator.getValue();

  float radians = angle * (std::acos(-1.0) / 180);
  camera.position = (Vector3){
    dist * std::cos(radians),
    height,
    dist * std::sin(radians)
  };
}
