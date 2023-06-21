#include "rasterizer.h"

#include "transformation.h"

void Rasterizer::render(const Model& model) {
    vShader->model = modelTransformation(model);
    vShader->view = viewTransformation(*camera);
    vShader->projection = projectionTransformation(*camera);
    vShader->viewport = viewportTransformation(*window);

    fShader->clearBuffer(0, 0, 0);
    for (const auto& mesh : model.meshes) {
        render(*mesh);
    }
}

void Rasterizer::render(const Mesh& mesh) {
    Mesh m(mesh);
    vShader->processVertices(m);

    for (auto& t : m.triangles) {
        fShader->loadTriangleVertices(m.vertices[t[0]], m.vertices[t[1]], m.vertices[t[2]]);
        fShader->rasterize();
    }

    window->updateFrameBufferFromImage(fShader->outputImage);
}

void Rasterizer::update(double deltaTime) {
    window->pollEvents();

    // Update camera
    auto fpsCamera = reinterpret_cast<FpsCamera*>(camera);
    if (window->keyPressed('W')) {
        fpsCamera->moveForward(deltaTime * fpsCamera->moveSpeed);
    }
    if (window->keyPressed('S')) {
        fpsCamera->moveForward(-deltaTime * fpsCamera->moveSpeed);
    }
    if (window->keyPressed('D')) {
        fpsCamera->moveRight(deltaTime * fpsCamera->moveSpeed);
    }
    if (window->keyPressed('A')) {
        fpsCamera->moveRight(-deltaTime * fpsCamera->moveSpeed);
    }

    auto dx = window->getCursorDeltaX();
    auto dy = window->getCursorDeltaY();
    fpsCamera->lookUp(-dy * fpsCamera->mouseSensitivity);
    fpsCamera->lookRight(dx * fpsCamera->mouseSensitivity);
}
