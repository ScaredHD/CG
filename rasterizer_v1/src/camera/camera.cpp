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
    return Mat4();
}