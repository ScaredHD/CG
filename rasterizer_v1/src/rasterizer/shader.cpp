#include "shader.h"
#include "mathutils.h"

void VertexShader::processVertices(Mesh& mesh) {
    auto t = viewport * projection * view * model;
    for (auto& [x, y, z, color] : mesh.vertices) {
        Vec3 p = hnormalized(t * homogeneous({{x, y, z}}));
        x = p[0];
        y = p[1];
        z = p[2];
    }
}

FragmentShader::FragmentShader(int w, int h) : outputImage(w, h) {
}

void FragmentShader::clearBuffer(Uchar r, Uchar g, Uchar b) {
    outputImage.fill({r, g, b, 255});
}

void FragmentShader::rasterize() {
    const auto& [x0, y0] = std::make_tuple(v0->x, v0->y);
    const auto& [x1, y1] = std::make_tuple(v1->x, v1->y);
    const auto& [x2, y2] = std::make_tuple(v2->x, v2->y);

    auto w = outputImage.width;
    auto h = outputImage.height;
    auto xmin = clamp(int(std::min({x0, x1, x2})), 0, w - 1);
    auto xmax = clamp(int(std::max({x0, x1, x2})), 0, w - 1);
    auto ymin = clamp(int(std::min({y0, y1, y2})), 0, h - 1);
    auto ymax = clamp(int(std::max({y0, y1, y2})), 0, h - 1);
    for (int i = xmin; i <= xmax; ++i) {
        for (int j = ymin; j <= ymax; ++j) {
            double x = i + 0.5;
            double y = j + 0.5;
            processFragments(x, y);
        }
    }
}

void FragmentShader::processFragments(int fragX, int fragY) {
    auto x = fragX + 0.5;
    auto y = fragY + 0.5;
    const auto& [x0, y0] = std::make_tuple(v0->x, v0->y);
    const auto& [x1, y1] = std::make_tuple(v1->x, v1->y);
    const auto& [x2, y2] = std::make_tuple(v2->x, v2->y);
    auto [alpha, beta] =
        barycentricCoordinates(Vec2{{x0, y0}}, Vec2{{x1, y1}}, Vec2{{x2, y2}}, Vec2{{x, y}});
    auto gamma = 1 - alpha - beta;

    if (alpha < 0 || alpha > 1) return;
    if (beta < 0 || beta > 1) return;
    if (alpha + beta > 1) return;

    auto c = alpha * v0->color + beta * v1->color + gamma * v2->color;
    VectorX<4, Uchar> color(c * 255.0);
    outputImage.setPixelValue(fragX, outputImage.height - 1 - fragY, color.arr);
}

void FragmentShader::loadTriangleVertices(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
    this->v0 = &v0;
    this->v1 = &v1;
    this->v2 = &v2;
}
