#include "transformation.h"

#include "mathutils.h"

Mat4 translateTransformation(double x, double y, double z) {
    // clang-format off
    return {{{
        {1, 0, 0, x},
        {0, 1, 0, y},
        {0, 0, 1, z},
        {0, 0, 0, 1}
    }}};
    // clang-fromat on
}

Mat4 translateTransformation(const Vec3& v) {
    return translateTransformation(v[0], v[1], v[2]);
}


Mat4 rotateX(double deg) {
    auto t = toRadian(deg);
    // clang-format off
    return {{{
        {1,           0,            0,   0},
        {0, std::cos(t), -std::sin(t),   0},
        {0, std::sin(t),  std::cos(t),   0},
        {0,           0,            0,   1}
    }}};
    // clang-fromat on
}

Mat4 rotateY(double deg) {
    auto t = toRadian(deg);
    // clang-format off
    return {{{
        { std::cos(t), 0, std::sin(t), 0},
        {           0, 1,           0, 0},
        {-std::sin(t), 0, std::cos(t), 0},
        {           0, 0,           0, 1}
    }}};
    // clang-fromat on
}

Mat4 rotateZ(double deg) {
    auto t = toRadian(deg);
    // clang-format off
    return {{{
        {std::cos(t), -std::sin(t), 0, 0},
        {std::sin(t),  std::cos(t), 0, 0},
        {          0,            0, 1, 0},
        {          0,            0, 0, 1}
    }}};
    // clang-fromat on
}

Mat4 rotateTransformation(double x, double y, double z) {
    return rotateX(x) * rotateY(y) * rotateZ(z);
}


Mat4 rotateTransformation(const Vec3& axis, double deg) {
    const auto& w = axis.normalized();
    const auto& [u, v] = alignedBasis(w);
    Mat3 rot({{
        {u[0], u[1], u[2]},
        {v[0], v[1], v[2]},
        {w[0], w[1], w[2]}
    }});
    auto res = rot.transposed() * linearPart(rotateZ(deg)) * rot;
    return homogeneousMatrix(res);
}

Mat4 scaleTransformation(double scaleX, double scaleY, double scaleZ) {
    // clang-format off
    return {{{
        {scaleX,       0,       0,   0},
        {     0,  scaleY,       0,   0},
        {     0,       0,  scaleZ,   0},
        {     0,       0,       0,   1}
    }}};
    // clang-fromat on
}

Mat4 modelTransformation(const Model& model) {
    auto m1 = translateTransformation(model.location);
    auto m2 = rotateTransformation(model.rotation[0], model.rotation[1], model.rotation[2]);
    auto m3 = scaleTransformation(model.scale[0], model.scale[1], model.scale[2]);
    return m1 * m2 * m3;
}
 

Mat4 viewTransformation(const Camera& cam) {
    const auto& w = -cam.lookAt.normalized();
    const auto& u = cross(cam.lookAt, cam.up).normalized();
    const auto& v = cross(w, u).normalized();
    Mat3 r({{{u[0], u[1], u[2]}, {v[0], v[1], v[2]}, {w[0], w[1], w[2]}}});
    return homogeneousMatrix(r) * translateTransformation(-cam.location);
}

Mat4 projectionTransformation(const Camera& cam) {
    const auto& [n, f, l, r, t, b] = std::make_tuple(cam.zNear, cam.zFar, cam.left(), cam.right(), cam.top(), cam.bottom());
    // clang-format off
    return Mat4({{
        {2 * n / (r - l),               0,  (l + r) / (l - r),                    0},
        {              0, 2 * n / (t - b),  (b + t) / (b - t),                    0},
        {              0,               0,  (f + n) / (n - f),  2 * f * n / (f - n)},
        {              0,               0,                  1,                    0}
    }});
    // clang-format on
}

Mat4 viewportTransformation(const Window& window) {
    double w = window.width;
    double h = window.height;
    // clang-format off
    return {{{
        {w / 2,     0, 0, w / 2},
        {    0, h / 2, 0, h / 2},
        {    0,     0, 1,     0},
        {    0,     0, 0,     1}
    }}};
    // clang-fromat on
}
