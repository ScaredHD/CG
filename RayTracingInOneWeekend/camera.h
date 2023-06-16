#pragma once

#include "vec.h"
#include "ray.h"
#include "rtweekend.h"

class Camera {
public:
    Camera(double vfov, double aspectRatio) {
        auto theta = toRadian(vfov);
        auto viewportHeight = 2.0 * std::tan(theta / 2);
        auto viewportWidth = aspectRatio * viewportHeight;
        auto focalLength = 1.0;

        origin = Vec3(0, 0, 0);
        horizontal = Vec3(viewportWidth, 0, 0);
        vertical = Vec3(0, viewportHeight, 0);
        lowerLeft = origin + Vec3(0, 0, -focalLength) - 0.5 * vertical - 0.5 * horizontal;

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