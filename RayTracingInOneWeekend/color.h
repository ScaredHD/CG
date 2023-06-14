#pragma once

#include <iostream>

#include "vec.h"

inline void writeColor(std::ostream& os, const Vec3& color) {
    os << static_cast<int>(255.999 * color.x()) << ' ' << static_cast<int>(255.999 * color.y())
       << ' ' << static_cast<int>(255.999 * color.z()) << '\n';
}