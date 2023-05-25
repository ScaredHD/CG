#pragma once

#include "geometry.h"
#include "image.h"

struct Shader {};

struct VertexShader : public Shader {
    Mat4 model;
    Mat4 view;
    Mat4 projection;
    Mat4 viewport;
    void processVertices(Mesh& mesh);
};

struct FragmentShader : public Shader {
    FragmentShader(int w, int h);
    void setTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);
    void rasterize();
    void processFragments(int fragX, int fragY);
    const Vertex* v0;
    const Vertex* v1;
    const Vertex* v2;
    RgbaImage outputImage;
};