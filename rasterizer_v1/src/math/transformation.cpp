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
        {1, 0,            0,           0},
        {0, std::cos(t), -std::sin(t), 0},
        {0, std::sin(t),  std::cos(t), 0},
        {0, 0,            0,           1}
    }}};
    // clang-fromat on
}

Mat4 rotateY(double deg) {
    auto t = toRadian(deg);
    // clang-format off
    return {{{
        { std::cos(t), 0, std::sin(t), 0},
        { 0,           1, 0,           0},
        {-std::sin(t), 0, std::cos(t), 0},
        { 0,           0, 0,           1}
    }}};
    // clang-fromat on
}

Mat4 rotateZ(double deg) {
    auto t = toRadian(deg);
    // clang-format off
    return {{{
        {std::cos(t), -std::sin(t), 0, 0},
        {std::sin(t),  std::cos(t), 0, 0},
        {0,            0,           1, 0},
        {0,            0,           0, 1}
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
        {scaleX, 0,      0,      0},
        {0,      scaleY, 0,      0},
        {0,      0,      scaleZ, 0},
        {0,      0,      0,      1}
    }}};
    // clang-fromat on
}
