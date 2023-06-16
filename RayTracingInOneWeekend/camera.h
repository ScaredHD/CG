#pragma once

#include "vec.h"
#include "ray.h"
#include "rtweekend.h"

class Camera {
  public:
    Camera(const Vec3& lookFrom, const Vec3& lookAt, const Vec3& vup, double vfov,
           double aspectRatio) {
        auto theta = toRadian(vfov);
        auto viewportHeight = 2.0 * std::tan(theta / 2);
        auto viewportWidth = aspectRatio * viewportHeight;
        auto focalLength = 1.0;

        auto w = normalized(lookFrom - lookAt);
        auto u = normalized(cross(vup, w));
        auto v = cross(w, u);

        origin = lookFrom;
        horizontal = viewportWidth * u;
        vertical = viewportHeight * v;
        lowerLeft = origin - 0.5 * horizontal - 0.5 * vertical - w;
    }

    Ray getRay(double u, double v) const {
        return Ray(origin, lowerLeft + u * horizontal + v * vertical - origin);
    }

  private:
    Vec3 origin;
    Vec3 lowerLeft;
    Vec3 horizontal;
    Vec3 vertical;
};