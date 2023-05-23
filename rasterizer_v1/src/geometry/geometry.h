#pragma once

#include <vector>
#include <array>

#include "vec.h"
#include "matrix.h"

struct Vertex {
    double x;
    double y;
    double z;
    Vec4 color;
};

struct Mesh {
    using VertexID = std::size_t;
    std::vector<Vertex> vertices;
    std::vector<std::array<VertexID, 3>> triangles;
};

struct Model {
    Mat4 modelTransformation() const;
    std::vector<Mesh*> meshes;
    Vec3 location;
    Vec3 rotation;
    Vec3 scale;
};