#include "testGeometry.h"

TestGeometry::TestGeometry() {
    Mesh mesh1;
    mesh1.vertices.push_back({1.0, 0, 0, Vec4{{1.0, 0, 0, 0}}});
    mesh1.vertices.push_back({0, 0, 0, Vec4{{0, 1.0, 0, 0}}});
    mesh1.vertices.push_back({0, 1.0, 0, Vec4{{0, 0, 1.0, 0}}});
    mesh1.triangles.push_back({0, 1, 2});
    Model model1;
    model1.meshes.push_back(&mesh1);
}