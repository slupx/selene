#include <raylib.h>
#include "camera.h"

int main() {
  InitWindow(1000, 800, "Selene");

  auto camera = sel::Camera();

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    camera.Update();

    BeginDrawing();
      ClearBackground(RAYWHITE);

      BeginMode3D(camera.getCamera());
        DrawGrid(10, 1.0);

        DrawCube((Vector3){0.0, 0.0, 0.0}, 1.5, 1.5, 1.5, BROWN);
        
      EndMode3D();
    EndDrawing(); 
  }

  return 0;
}
