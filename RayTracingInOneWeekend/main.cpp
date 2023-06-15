#include <iostream>

#include "rtweekend.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "camera.h"

RandomGenerator gen;

Vec3 rayColor(const Ray& r, const HittableList& world, int maxDepth) {
    if (maxDepth <= 0) return Vec3(0, 0, 0);

    HitRecord rec;
    if (world.hit(r, 0.0, infinity, rec)) {
        auto outDir = rec.normal + gen.randomVec3InUnitSphere();
        return 0.5 * rayColor(Ray(rec.p, outDir), world, maxDepth - 1);
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
    const int samplesPerPixel = 100;
    const int maxDepth = 50;

    // Camera
    Camera cam;

    // Objects
    HittableList world;
    world.add(std::make_shared<Sphere>(Vec3{0, 0, -1}, 0.5));
    world.add(std::make_shared<Sphere>(Vec3{0, -100.5, -1}, 100));

    // Render
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < imageWidth; ++i) {
            Vec3 color(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s) {
                auto u = (i + gen.randomDouble()) / (imageWidth - 1);
                auto v = (j + gen.randomDouble()) / (imageHeight - 1);
                Ray r = cam.getRay(u, v);
                color += rayColor(r, world, maxDepth);
            }

            writeColor(std::cout, color, samplesPerPixel);
        }
    }
    std::cerr << "\nDone.\n";
}