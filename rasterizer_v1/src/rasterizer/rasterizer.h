#pragma once

#include "camera.h"
#include "window.h"
#include "geometry.h"
#include "shader.h"

#include <memory>

class Rasterizer {
  public:
    void render(const Model& model);
    void render(const Mesh& mesh);

    void update(double deltaTime);
    void drawFrameBufferToWindow() { window->drawFrameBuffer(); }

    std::shared_ptr<Camera> camera;
    Window* window;  // Using shared_ptr introduces bugs I can't fix
    std::shared_ptr<VertexShader> vShader;
    std::shared_ptr<FragmentShader> fShader;
};