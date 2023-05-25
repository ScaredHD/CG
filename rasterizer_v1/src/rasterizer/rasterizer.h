#pragma once

#include "camera.h"
#include "window.h"
#include "geometry.h"
#include "shader.h"

class Rasterizer {
  public:
    void render(const Model& model);
    void render(const Mesh& mesh);
    void drawScreenSpaceTriangle(const Vec2& a, const Vec2& b, const Vec2& c);

    Camera* camera;
    Window* window;
    VertexShader* vShader;
    FragmentShader* fShader;
};