#pragma once

#include "vec.h"

struct Ray {
  public:
    Ray() = default;
    Ray(const Vec3& o, const Vec3& d, double time = 0.0) : o{o}, d{d}, time{time} {}

    Vec3 at(double t) const { return o + t * d; }

    Vec3 o;  // origin
    Vec3 d;  // direction
    double time;
};