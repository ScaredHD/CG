#include "rasterizer.h"

#include "transformation.h"

void Rasterizer::render(const Model& model) {
    vShader->model = model.modelTransformation();
    vShader->view = camera->viewTransformation();
    vShader->projection = camera->projectionTransformation();
    vShader->viewport = window->viewportTransformation();
    for (const auto& mesh : model.meshes) {
        render(*mesh);
    }
}

void Rasterizer::render(const Mesh& mesh) {
    Mesh m(mesh);
    vShader->processVertices(m);
    for (auto& t : m.triangles) {
        fShader->setTriangle(m.vertices[t[0]], m.vertices[t[1]], m.vertices[t[2]]);
        fShader->rasterize();
    }
    window->updateFrameBufferFromImage(fShader->outputImage);
}
