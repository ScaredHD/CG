#include <iostream>

#include "rtweekend.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"

template <typename T>
T lerp(const T& a, const T& b, double t) {
    return (1 - t) * a + t * b;
}

Vec3 rayColor(const Ray& r, const HittableList& world) {
    HitRecord rec;
    if (world.hit(r, 0.0, infinity, rec)) {
        return 0.5 * (rec.normal + Vec3(1, 1, 1));
    } else {
        auto u = normalized(r.d);
        auto t = (u.y() + 1.0) * 0.5;
        return lerp(Vec3(1.0, 1.0, 1.0), Vec3(0.5, 0.7, 1.0), t);
    }
}

int main() {
    // Image
    const double aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

    // Camera
    auto viewportHeight = 2.0;
    auto viewportWidth = aspectRatio * viewportHeight;
    const double focalLength = 1.0;

    auto origin = Vec3(0, 0, 0);
    auto horizontal = Vec3(viewportWidth, 0, 0);
    auto vertical = Vec3(0, viewportHeight, 0);
    auto lowerLeft = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focalLength);

    // Objects
    HittableList world;
    world.add(std::make_shared<Sphere>(Vec3{0, 0, -1}, 0.5));
    world.add(std::make_shared<Sphere>(Vec3{0, -100.5, -1}, 100));

    // Render
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < imageWidth; ++i) {
            auto u = double(i) / (imageWidth - 1);
            auto v = double(j) / (imageHeight - 1);
            Ray r(origin, lowerLeft + u * horizontal + v * vertical - origin);

            writeColor(std::cout, rayColor(r, world));
        }
    }
    std::cerr << "\nDone.\n";
}