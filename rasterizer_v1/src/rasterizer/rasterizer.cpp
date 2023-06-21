#include "rasterizer.h"

#include "transformation.h"

void Rasterizer::render(const Model& model) {
    vShader->model = modelTransformation(model);
    vShader->view = viewTransformation(*camera);
    vShader->projection = projectionTransformation(*camera);
    vShader->viewport = window->viewportTransformation();

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
