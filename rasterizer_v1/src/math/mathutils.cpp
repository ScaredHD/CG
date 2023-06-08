#include "mathutils.h"

#include <limits>

const double pi = std::acos(-1);
const double epsilon = std::numeric_limits<double>::epsilon();

CartesianCoordinates::CartesianCoordinates(const Vec3& p) : x(p[0]), y(p[1]), z(p[2]) {
}

CartesianCoordinates::CartesianCoordinates(const SphericalCoordinates& p) {
    x = p.r * std::sin(p.theta) * std::cos(p.phi);
    y = p.r * std::cos(p.theta);
    z = p.r * std::sin(p.theta) * std::sin(p.phi);
}

SphericalCoordinates::SphericalCoordinates(const CartesianCoordinates& p) {
    r = std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
    phi = std::atan2(p.z, p.x);
    theta = std::acos(p.y / r); 
}

double toRadian(double deg) {
    return deg * pi / 180.0;
}

double toDegree(double rad) {
    return rad * 180.0 / pi;
}

std::tuple<double, double> barycentricCoordinates(const Vec2& v0, const Vec2& v1, const Vec2& v2,
                                                  const Vec2& p) {
    const auto& [x0, y0, x1, y1, x2, y2, x, y] =
        std::make_tuple(v0[0], v0[1], v1[0], v1[1], v2[0], v2[1], p[0], p[1]);

    auto D = (x0 - x2) * (y1 - y2) - (x1 - x2) * (y0 - y2);
    
    if (std::abs(D) < epsilon) {
        return std::make_tuple(-1.0, -1.0);
    }

    auto alpha = (x - x2) * (y1 - y2) - (x1 - x2) * (y - y2);
    alpha /= D;

    auto beta = (x0 - x2) * (y - y2) - (x - x2) * (y0 - y2);
    beta /= D;

    return std::make_tuple(alpha, beta);
}

std::tuple<Vec3, Vec3> alignedBasis(const Vec3& wAlignedTo) {
    auto w = wAlignedTo.normalized();
    auto minComponent = std::min({w[0], w[1], w[2]});
    auto temp = w;
    for (auto& c : temp.arr) {
        if (c == minComponent) {
            c = minComponent;
            break;
        }
    }
    auto v = cross(w, temp).normalized();
    auto u = cross(v, w).normalized();
    return {u, v};
}

double lerp(double a, double b, double t) {
    return a + t * (b - a);
}