#pragma once

#include "rtweekend.h"

class AABB {
  public:
    AABB() = default;
    AABB(const Vec3& a, const Vec3& b) : a{a}, b{b} {}

    bool hit(const Ray& r, double tmin, double tmax) const {
        for (int dim = 0; dim < 3; ++dim) {
            auto invD = 1.0 / r.d[dim];
            auto t0 = (a[dim] - r.o[dim]) * invD;
            auto t1 = (b[dim] - r.o[dim]) * invD;
            if (invD < 0.0) std::swap(t0, t1);
            tmin = std::max(tmin, t0);
            tmax = std::min(tmax, t1);
            if (tmax <= tmin) return false;
        }
        return true;
    }

    Vec3 a;
    Vec3 b;
};

inline AABB surroundingBox(const AABB& box0, const AABB& box1) {
    Vec3 a(std::min(box0.a.x(), box1.a.x()), std::min(box0.a.y(), box1.a.y()),
           std::min(box0.a.z(), box1.a.z()));
    Vec3 b(std::max(box0.b.x(), box1.b.x()), std::max(box0.b.y(), box1.b.z()),
           std::max(box0.b.z(), box1.b.z()));
    return {a, b};
}