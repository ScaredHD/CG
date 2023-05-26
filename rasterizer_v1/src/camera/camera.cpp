#include "camera.h"

#include <tuple>
#include <cmath>

#include "mathutils.h"
#include "transformation.h"

double Camera::pitch() const {
    auto d = toDegree(SphericalCoordinates(CartesianCoordinates(lookAt)).theta);
    return 90.0 - d;
}

double Camera::yaw() const {
    return toDegree(SphericalCoordinates(CartesianCoordinates(lookAt)).phi);
}

void FpsCamera::moveForward(double dist) {
    location += dist * lookAt.normalized();
}

void FpsCamera::moveRight(double dist) {
    auto right = cross(lookAt.normalized(), up.normalized());
    location += dist * right;
}

void FpsCamera::lookUp(double deg) {
    auto s = SphericalCoordinates(CartesianCoordinates(lookAt));
    s.theta -= toRadian(deg);
    auto c = CartesianCoordinates(s);
    lookAt = {{c.x, c.y, c.z}};
}

void FpsCamera::lookRight(double deg) {
    auto s = SphericalCoordinates(CartesianCoordinates(lookAt));
    s.phi += toRadian(deg);
    if (s.phi > pi) s.phi -= 2 * pi;
    if (s.phi < -pi) s.phi += 2 * pi;
    auto c = CartesianCoordinates(s);
    lookAt = {{c.x, c.y, c.z}};
}

Mat4 Camera::viewTransformation() const {
    const auto& w = -lookAt.normalized();
    const auto& u = cross(lookAt, up).normalized();
    const auto& v = cross(w, u).normalized();
    Mat3 r({{{u[0], u[1], u[2]}, {v[0], v[1], v[2]}, {w[0], w[1], w[2]}}});
    return homogeneousMatrix(r) * translateTransform(-location);
}

Mat4 Camera::projectionTransformation() const {
    const auto& [n, f, l, r, t, b] = std::make_tuple(zNear, zFar, left(), right(), top(), bottom());
    // clang-format off
    return Mat4({{
        {2 * n / (r - l),               0,  (l + r) / (l - r),                    0},
        {              0, 2 * n / (t - b),  (b + t) / (b - t),                    0},
        {              0,               0,  (f + n) / (n - f),  2 * f * n / (f - n)},
        {              0,               0,                  1,                    0}
    }});
    // clang-format on
}

double Camera::top() const {
    return right() / aspectRatio;
}

double Camera::bottom() const {
    return -top();
}

double Camera::left() const {
    return -right();
}

double Camera::right() const {
    return std::tan(toRadian(hFovDeg) / 2) * std::abs(zNear);
}
