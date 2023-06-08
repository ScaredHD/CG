#pragma once

#include "camera.h"
#include "window.h"
#include "geometry.h"
#include "shader.h"

class Rasterizer {
  public:
    void render(const Model& model);
    void render(const Mesh& mesh);

    void drawLine(const Vec2& start, const Vec2& end);

    Camera* camera;
    Window* window;
    VertexShader* vShader;
    FragmentShader* fShader;
};