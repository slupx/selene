#include "camera.hpp"
#include <raylib.h>
#include "raymath.h"
#include "rlgl.h"

// Load custom render texture, create a writable depth texture buffer
RenderTexture2D LoadRenderTextureDepthTex(int width, int height) {
    RenderTexture2D target = { 0 };

    target.id = rlLoadFramebuffer();
    if (target.id == 0) return target;
    rlEnableFramebuffer(target.id);

    // Create color texture
    target.texture.id = rlLoadTexture(nullptr, width, height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
    target.texture.width = width;
    target.texture.height = height;
    target.texture.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    target.texture.mipmaps = 1;
    rlFramebufferAttach(target.id, target.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);

    // Create depth texture
    target.depth.id = rlLoadTextureDepth(width, height, false); // false = create as depth texture
    target.depth.width = width;
    target.depth.height = height;
    target.depth.format = 19; // DEPTH_COMPONENT
    target.depth.mipmaps = 1;
    rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

    rlDisableFramebuffer();
    return target;
}

// Unload render texture from GPU memory
void UnloadRenderTextureDepthTex(RenderTexture2D target) {
    if (target.id > 0) rlUnloadFramebuffer(target.id);
    if (target.texture.id > 0) rlUnloadTexture(target.texture.id);
    if (target.depth.id > 0) rlUnloadTexture(target.depth.id);
}

int main() {
  const int WIDTH = 1000, HEIGHT = 800, PIXEL_SCALE = 4;

  using namespace std::chrono_literals;

  InitWindow(1000, 800, "Selene");

  // shader load
  const char *appDir = GetApplicationDirectory();

  Shader baseShader = LoadShader(
    TextFormat("%sshaders/base.vs", appDir),
    TextFormat("%sshaders/base.fs", appDir)
  );
  int timeLoc = GetShaderLocation(baseShader, "time");
  
  
  Shader postShader = LoadShader(
    nullptr,
    TextFormat("%sshaders/post.fs", appDir)
  );
  int pixelSizeLoc = GetShaderLocation(postShader, "pixelSize");


  Shader groundShader = LoadShader(
    TextFormat("%sshaders/ground.vs", appDir),
    TextFormat("%sshaders/ground.fs", appDir)
  );
  groundShader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(groundShader, "instanceTransform");

  // init
  RenderTexture2D gBuffer = LoadRenderTextureDepthTex(WIDTH / PIXEL_SCALE, HEIGHT / PIXEL_SCALE);
  int depthTextureLoc = GetShaderLocation(postShader, "depthTexture");
  auto camera = sel::Camera();
  SetTargetFPS(60);


  // Create a sphere model with properly generated vertex normals
  Mesh sphereMesh = GenMeshSphere(1.0f, 32, 32);
  Model sphereModel = LoadModelFromMesh(sphereMesh);
  sphereModel.materials[0].shader = baseShader;

  Model stoneModel = LoadModel("./resources/models/stone_1.obj");
  stoneModel.materials[0].shader = baseShader;

  std::vector<Matrix> stoneTransforms = {
    MatrixMultiply(MatrixTranslate(0, 0.3, 0), MatrixMultiply(MatrixRotateY(0.3), MatrixScale(1, 1.1, 1))),
    MatrixMultiply(MatrixTranslate(4, 0.3, 3), MatrixMultiply(MatrixRotateY(2), MatrixScale(0.7, 0.8, 0.7))),
    MatrixMultiply(MatrixTranslate(-3, 0.3, -5), MatrixMultiply(MatrixRotateY(1), MatrixScale(0.8, 0.9, 0.8))),
    MatrixMultiply(MatrixTranslate(5, 0.3, 4), MatrixMultiply(MatrixRotateY(-0.5), MatrixScale(0.7, 0.8, 0.9))),
  };


  float time = 0;
  
  // Mesh groundMesh = GenMeshPlane(1, 1, 3, 4);
  Mesh groundMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
  Model groundModel = LoadModelFromMesh(groundMesh);
  groundModel.materials[0].shader = groundShader;
  
  const int TILE_COUNT = 16;
  std::vector<Matrix> transforms(TILE_COUNT * TILE_COUNT);
  
  for (int x = 0; x < TILE_COUNT; x++) {
    for (int z = 0; z < TILE_COUNT; z++) {
      float height = -0.4 + (float)(sin(x) + sin(z)) * 0.2f; 
      
      transforms[x * TILE_COUNT + z] = MatrixTranslate(
        (float)x - (float)TILE_COUNT / 2.0f,
        height,
        (float)z - (float)TILE_COUNT / 2.0f
      );
    }
  }
  float a = 1 / (float)HEIGHT * (float)PIXEL_SCALE;
  SetShaderValue(postShader, pixelSizeLoc, &a, SHADER_UNIFORM_FLOAT);
  
  while (!WindowShouldClose()) {
    camera.Update();
    
    time += 0.02;
    SetShaderValue(baseShader, timeLoc, &time, SHADER_UNIFORM_FLOAT);

    BeginTextureMode(gBuffer);
      ClearBackground((Color){ 212, 222, 242, 255 });
      BeginShaderMode(baseShader);
        BeginMode3D(camera.getCamera());

          DrawMeshInstanced(
            groundMesh,
            groundModel.materials[0],
            transforms.data(),
            TILE_COUNT * TILE_COUNT
          );

          for (int i = 0; i < stoneTransforms.size(); i++) {
            DrawMesh(
              stoneModel.meshes[0],
              stoneModel.materials[0],
              stoneTransforms[i]
            );
          }

          DrawModel(sphereModel, Vector3{-5, 2, -5}, 1.0f, SKYBLUE);
        EndMode3D();
      EndShaderMode();
    EndTextureMode();

    BeginDrawing();
      // ClearBackground(RAYWHITE);
      BeginShaderMode(postShader);
        // Bind the depth texture to OpenGL slot 1
        rlActiveTextureSlot(1);
        rlEnableTexture(gBuffer.depth.id);
        
        int depthTextureSlot = 1;
        SetShaderValue(postShader, depthTextureLoc, &depthTextureSlot, SHADER_UNIFORM_INT);
        
        // Restore active texture unit to 0 for drawing the main texture
        rlActiveTextureSlot(0);

        DrawTexturePro(
          gBuffer.texture,
          Rectangle{0, 0,
            (float)gBuffer.texture.width,
            (float)-gBuffer.texture.height
          },
          Rectangle{0, 0,
            (float)WIDTH,
            (float)-HEIGHT
          },
          Vector2{0, 0}, 0, WHITE
        );

      EndShaderMode();

      DrawFPS(10, 10);
    EndDrawing();
  }

  // Cleanup resources
  UnloadModel(sphereModel);
  UnloadModel(groundModel);
  UnloadRenderTextureDepthTex(gBuffer);
  UnloadShader(baseShader);
  UnloadShader(postShader);
  UnloadShader(groundShader);

  CloseWindow();

  return 0;
}
