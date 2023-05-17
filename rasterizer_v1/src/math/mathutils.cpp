#include "mathutils.h"

const double pi = std::acos(-1);

CartesianCoordinates::CartesianCoordinates(const Vec3& p) : x(p.x()), y(p.y()), z(p.z()) {
}

CartesianCoordinates::CartesianCoordinates(const SphericalCoordinates& p) {
    x = p.r * std::sin(p.theta) * std::cos(p.phi);
    y = p.r * std::cos(p.theta);
    z = p.r * std::sin(p.theta) * std::sin(p.phi);
}

SphericalCoordinates::SphericalCoordinates(const CartesianCoordinates& p) {
    r = std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
    phi = std::atan2(p.z, p.x);
    theta = std::acos(p.y / std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z));
}

double toRadian(double deg) {
    return deg * pi / 180.0;
}

double toDegree(double rad) {
    return rad * 180.0 / pi;
}