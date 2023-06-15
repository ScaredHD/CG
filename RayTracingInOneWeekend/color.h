#pragma once

#include <iostream>

#include "vec.h"
#include "rtweekend.h"

inline void writeColor(std::ostream& os, const Vec3& color, int samplesPerPixel) {
    auto r = std::sqrt(color.x() / samplesPerPixel);
    auto g = std::sqrt(color.y() / samplesPerPixel);
    auto b = std::sqrt(color.z() / samplesPerPixel);


    os << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
       << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
       << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}