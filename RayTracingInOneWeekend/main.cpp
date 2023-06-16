#include <iostream>

#include "rtweekend.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"

using namespace std;

Vec3 rayColor(const Ray& r, const HittableList& world, int maxDepth) {
    if (maxDepth <= 0) return Vec3(0, 0, 0);

    HitRecord rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        Ray scattered;
        Vec3 attenuation;
        if (rec.material->scatter(r, rec, attenuation, scattered)) {
            return attenuation * rayColor(scattered, world, maxDepth - 1);
        }
        return Vec3(0, 0, 0);
    }

    auto u = normalized(r.d);
    auto t = (u.y() + 1.0) * 0.5;
    return lerp(Vec3(1.0, 1.0, 1.0), Vec3(0.5, 0.7, 1.0), t);
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

    // clang-format off
    auto MaterialGround = make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0));
    auto MaterialCenter = make_shared<Lambertian>(Vec3(0.1, 0.2, 0.5));
    auto MaterialLeft = make_shared<Dielectric>(1.5);
    auto MaterialRight = make_shared<Metal>(Vec3(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<Sphere>(Vec3{ 0.0, -100.5, -1.0}, 100, MaterialGround));
    world.add(make_shared<Sphere>(Vec3{ 0.0,    0.0, -1.0}, 0.5, MaterialCenter));
    world.add(make_shared<Sphere>(Vec3{-1.0,    0.0, -1.0}, 0.5, MaterialLeft));
    world.add(make_shared<Sphere>(Vec3{-1.0,    0.0, -1.0}, -0.4, MaterialLeft));
    world.add(make_shared<Sphere>(Vec3{ 1.0,    0.0, -1.0}, 0.5, MaterialRight));
    // clang-format on

    // Render
    cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; --j) {
        cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < imageWidth; ++i) {
            Vec3 color(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s) {
                auto u = (i + gen.randomDouble()) / imageWidth;
                auto v = (j + gen.randomDouble()) / imageHeight;
                Ray r = cam.getRay(u, v);
                color += rayColor(r, world, maxDepth);
            }

            writeColor(cout, color, samplesPerPixel, true);
        }
    }
    cerr << "\nDone.\n";
}