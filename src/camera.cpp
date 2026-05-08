#include "camera.h"
#include <cmath>
#include <raylib.h>

sel::Camera::Camera() : camera{ 0.0 }, angle{0.0} {
  camera.position = { 0, 10, 10 };
  camera.up = (Vector3){ 0.0, 1.0, 0.0 };
  camera.fovy = 45;
  camera.target = (Vector3){ 0.0, 0.0, 0.0 };
}

Camera3D sel::Camera::getCamera() { return camera; }

void sel::Camera::Update() {
 if (IsKeyDown(KEY_LEFT)) angle -= 0.02;
 if (IsKeyDown(KEY_RIGHT)) angle += 0.02;
 

 camera.position = (Vector3){ 10 * std::cos(angle), 10, 10 * std::sin(angle)};
}
