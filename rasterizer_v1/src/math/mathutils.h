#pragma once

#include <cmath>
#include <tuple>

#include "vec.h"

extern const double pi;
extern const double epsilon;

// coordinate system: right-handed, y-up

struct SphericalCoordinates;

struct CartesianCoordinates {
    CartesianCoordinates(double x, double y, double z) : x(x), y(y), z(z) {}
    CartesianCoordinates(const SphericalCoordinates& p);
    CartesianCoordinates(const Vec3& p);
    double x;
    double y;
    double z;
};

struct SphericalCoordinates {
    SphericalCoordinates(double r, double phi, double theta) : r(r), phi(phi), theta(theta) {}
    SphericalCoordinates(const CartesianCoordinates& p);
    double r;
    double phi;    // azimuthal angle
    double theta;  // polar angle (polar axis: y)
};

std::tuple<double, double> barycentricCoordinates(const Vec2& v0, const Vec2& v1, const Vec2& v2, const Vec2& p);

double toRadian(double deg);
double toDegree(double rad);

double clamp(double val, double min, double max);

std::tuple<Vec3, Vec3> alignedBasis(const Vec3& wAlignedTo);
