#include "camera.h"

#include <tuple>
#include <cmath>

#include "mathutils.h"

double Camera::pitch() const {
    return 90.0 - toDegree(SphericalCoordinates(CartesianCoordinates(lookAt)).theta);
}

double Camera::yaw() const {
    return toDegree(SphericalCoordinates(CartesianCoordinates(lookAt)).phi);
}

void FpsCamera::moveForward(double delta) {
    location += moveSpeed * delta * lookAt.normalized();
}

void FpsCamera::moveRight(double delta) {
    location = moveSpeed * delta * cross(lookAt.normalized(), up.normalized());
}

void FpsCamera::lookUp(double delta) {
    auto s = SphericalCoordinates(CartesianCoordinates(lookAt));
    s.theta -= toRadian(mouseSensitivity * delta);
    auto c = CartesianCoordinates(s);
    lookAt = Vec3(c.x, c.y, c.z);
}

void FpsCamera::lookRight(double delta) {
    auto s = SphericalCoordinates(CartesianCoordinates(lookAt));
    s.phi += toRadian(mouseSensitivity * delta);
    if (s.phi > pi) s.phi -= 2 * pi;
    if (s.phi < -pi) s.phi += 2 * pi;
    auto c = CartesianCoordinates(s);
    lookAt = Vec3(c.x, c.y, c.z);
}
