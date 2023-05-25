#include "transformation.h"

#include "mathutils.h"

Mat4 translateTransform(double x, double y, double z) {
    // clang-format off
    return {{{
        {1, 0, 0, x},
        {0, 1, 0, y},
        {0, 0, 1, z},
        {0, 0, 0, 1}
    }}};
    // clang-fromat on
}

Mat4 translateTransform(const Vec3& v) {
    return translateTransform(v[0], v[1], v[2]);
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

Mat4 rotateTransform(double x, double y, double z) {
    return rotateX(x) * rotateY(y) * rotateZ(z);
}


Mat4 rotateTransform(const Vec3& axis, double deg) {
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

Mat4 scaleTransform(double scaleX, double scaleY, double scaleZ) {
    // clang-format off
    return {{{
        {scaleX, 0,      0,      0},
        {0,      scaleY, 0,      0},
        {0,      0,      scaleZ, 0},
        {0,      0,      0,      1}
    }}};
    // clang-fromat on
}
