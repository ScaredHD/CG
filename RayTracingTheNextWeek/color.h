#pragma once

#include <iostream>

#include "vec.h"
#include "rtweekend.h"

inline void writeColor(std::ostream& os, const Vec3& color, int samplesPerPixel,
                       bool gammaCorrection) {
    auto r = color.x() / samplesPerPixel;
    auto g = color.y() / samplesPerPixel;
    auto b = color.z() / samplesPerPixel;

    if (gammaCorrection) {
        r = std::sqrt(r);
        g = std::sqrt(g);
        b = std::sqrt(b);
    }

    os << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
       << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
       << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}