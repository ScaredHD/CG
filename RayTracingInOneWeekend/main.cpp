#include <iostream>

#include "vec.h"
#include "color.h"
#include "ray.h"

template <typename T>
T lerp(const T& a, const T& b, double t) {
    return (1 - t) * a + t * b;
}

double hitSphere(const Ray& r, const Vec3& center, double radius) {
    auto oc = r.o - center;
    auto a = dot(r.d, r.d);
    auto b = 2.0 * dot(r.d, oc);
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    if (discriminant < 0.0) {
        return -1.0;
    }
    return -(b + std::sqrt(discriminant)) / (2 * a);
}

Vec3 rayColor(const Ray& r) {
    auto center = Vec3(0, 0, -1);
    auto radius = 0.5;
    auto t = hitSphere(r, center, radius);
    if (t > 0.0) {
        auto normal = normalized(r.at(t) - center);
        normal += {1.0, 1.0, 1.0};
        normal *= 0.5;
        return normal;
    } else {
        auto u = normalized(r.d);
        t = (u.y() + 1.0) * 0.5;
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

    // Render
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < imageWidth; ++i) {
            auto u = double(i) / (imageWidth - 1);
            auto v = double(j) / (imageHeight - 1);
            Ray r(origin, lowerLeft + u * horizontal + v * vertical - origin);

            writeColor(std::cout, rayColor(r));
        }
    }
    std::cerr << "\nDone.\n";
}