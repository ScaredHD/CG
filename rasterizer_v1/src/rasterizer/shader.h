#pragma once

#include "geometry.h"

struct Shader {

};

struct VertexShader : public Shader {
    void processVertices(Mesh& mesh);
};

struct FragmentShader : public Shader {
};