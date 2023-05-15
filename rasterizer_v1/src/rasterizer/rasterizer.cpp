#include "rasterizer.h"

void Rasterizer::render(const Model& model) {
    for (const auto& mesh : model.meshes) {
        render(*mesh);
    }
}

void Rasterizer::render(const Mesh& mesh) {
    Mesh m(mesh);
    vShader->processVertices(m);
    for (auto& t : m.triangles) {
    }
}

void Rasterizer::renderConstantColor(Color c) {
    int w = window->width();
    int h = window->height();
    auto frameBuffer = std::make_shared<Buffer2D>(w, h, c);
    window->updateFrameBuffer(frameBuffer);
    window->displayOnScreen();
}
