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

void FpsCamera::moveForward(double dist) {
    location += dist * lookAt.normalized();
}

void FpsCamera::moveRight(double dist) {
    location = dist * cross(lookAt.normalized(), up.normalized());
}

void FpsCamera::lookUp(double deg) {
    auto s = SphericalCoordinates(CartesianCoordinates(lookAt));
    s.theta -= toRadian(deg);
    auto c = CartesianCoordinates(s);
    lookAt = Vec3(c.x, c.y, c.z);
}

void FpsCamera::lookRight(double deg) {
    auto s = SphericalCoordinates(CartesianCoordinates(lookAt));
    s.phi += toRadian(deg);
    if (s.phi > pi) s.phi -= 2 * pi;
    if (s.phi < -pi) s.phi += 2 * pi;
    auto c = CartesianCoordinates(s);
    lookAt = Vec3(c.x, c.y, c.z);
}
