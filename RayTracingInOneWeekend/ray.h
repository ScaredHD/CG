#pragma once

#include "vec.h"

struct Ray {
    Ray(const Vec3& o, const Vec3& d) : o(o), d(d) {}

    Vec3 at(double t) const { return o + t * d; }

    Vec3 o;  // origin
    Vec3 d;  // direction
};