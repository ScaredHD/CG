#pragma once

#include <cmath>

#include "vec.h"

extern const double pi;

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


double toRadian(double deg);
double toDegree(double rad);

double clamp(double val, double min, double max);