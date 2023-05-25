#include "vec.h"


Vec4 homogeneous(const Vec3& v) {
    return {{v[0], v[1], v[2], 1.0}};
}

Vec3 hnormalized(const Vec4& v) {
    return Vec3{{v[0], v[1], v[2]}} / v[3];
}