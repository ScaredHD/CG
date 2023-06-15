#pragma once

#include <cmath>
#include <limits>
#include <memory>

#include "ray.h"
#include "vec.h"

constexpr double infinity = std::numeric_limits<double>::infinity();
const double pi = std::acos(-1);

inline double toRadian(double deg) {
    return deg * pi / 180.0;
}

