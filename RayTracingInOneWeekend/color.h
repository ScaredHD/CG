#pragma once

#include <iostream>

#include "vec.h"
#include "rtweekend.h"

inline void writeColor(std::ostream& os, const Vec3& color, int samplesPerPixel) {
    auto output = color;
    output *= 1.0 / samplesPerPixel;

    os << static_cast<int>(256 * clamp(output.x(), 0.0, 0.999)) << ' '
       << static_cast<int>(256 * clamp(output.y(), 0.0, 0.999)) << ' '
       << static_cast<int>(256 * clamp(output.z(), 0.0, 0.999)) << '\n';
}