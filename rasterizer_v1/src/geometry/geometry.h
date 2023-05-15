#pragma once

#include <vector>
#include <array>

#include "vec.h"

struct Mesh {
    using VertexID = std::size_t;
    std::vector<Vec3> vertices;
    std::vector<std::array<VertexID, 3>> triangles;
};

struct Model {
    std::vector<Mesh*> meshes;
};