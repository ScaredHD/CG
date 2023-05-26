#include "testGeometry.h"

TestGeometry::TestGeometry() {
    mesh1.vertices.push_back({1.0, 0, 0, Vec4{{1.0, 0, 0, 0}}});
    mesh1.vertices.push_back({0, 0, 0, Vec4{{0, 1.0, 0, 0}}});
    mesh1.vertices.push_back({0, 1.0, 0, Vec4{{0, 0, 1.0, 0}}});
    mesh1.triangles.push_back({0, 1, 2});
    triangle1.meshes.push_back(&mesh1);
    triangle1.location = Vec3({0, 0, -2});
    triangle1.rotation = Vec3({0, 0, 0});
    triangle1.scale = Vec3({1., 1., 1.});
}