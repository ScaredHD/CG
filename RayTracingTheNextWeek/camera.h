#pragma once

#include "vec.h"
#include "ray.h"
#include "rtweekend.h"

class Camera {
  public:
    Camera(const Vec3& lookFrom, const Vec3& lookAt, const Vec3& vup, double vfov,
           double aspectRatio, double aperture, double focusDistance) {
        auto theta = toRadian(vfov);
        auto viewportHeight = 2.0 * std::tan(theta / 2);
        auto viewportWidth = aspectRatio * viewportHeight;
        auto focalLength = 1.0;

        w = normalized(lookFrom - lookAt);
        u = normalized(cross(vup, w));
        v = cross(w, u);

        origin = lookFrom;
        horizontal = focusDistance * viewportWidth * u;
        vertical = focusDistance * viewportHeight * v;
        lowerLeft = origin - 0.5 * horizontal - 0.5 * vertical - focusDistance * w;

        lensRadius = aperture / 2;
    }

    Ray getRay(double s, double t) const {
        Vec3 rd = lensRadius * gen.randomInUnitDisk();
        Vec3 offset = u * rd.x() + v * rd.y();
        return Ray(origin + offset, lowerLeft + s * horizontal + t * vertical - origin - offset);
    }

  private:
    Vec3 origin;
    Vec3 lowerLeft;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    double lensRadius;
};