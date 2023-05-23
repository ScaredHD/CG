#include "shader.h"

void VertexShader::processVertices(Mesh& mesh) {
    auto t = viewport * projection * view * model;
    for (auto& [x, y, z, color] : mesh.vertices) {
        // VectorX<4> p({x, y, z, 1.0});
        // p = t * p;
    }
}

void FragmentShader::processFragments() {

}

void FragmentShader::setTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
    this->v0 = &v0;
    this->v1 = &v1;
    this->v2 = &v2;
}
