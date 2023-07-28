#include <iostream>

#include "rtweekend.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "camera.h"

using namespace std;

Vec3 rayColor(const Ray& r, const HittableList& world, int maxDepth) {
    if (maxDepth <= 0) return {0, 0, 0};

    HitRecord rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        Ray scattered;
        Vec3 attenuation;
        if (rec.material->scatter(r, rec, attenuation, scattered)) {
            return attenuation * rayColor(scattered, world, maxDepth - 1);
        }
        return {0, 0, 0};
    }

    auto u = normalized(r.d);
    auto t = (u.y() + 1.0) * 0.5;
    return lerp(Vec3{1.0, 1.0, 1.0}, Vec3{0.5, 0.7, 1.0}, t);
}

int main() {
    // Image
    const double aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 32;
    const int maxDepth = 32;

    // Objects
    HittableList world;

    // Camera
    Vec3 lookFrom;
    Vec3 lookAt = {0, 0, 0};
    Vec3 vup = {0, 1, 0};
    double vFov = 40.0;
    auto distToFocus = 10.0;
    auto aperture = 0.0;
    const auto& [t0, t1] = std::make_tuple(0.0, 1.0);

    switch (0) {
        case 1:
            world = randomScene();
            lookFrom = {13, 2, 3};
            lookAt = {0, 0, 0};
            vFov = 20.0;
            aperture = 0.1;
            break;

        case 2:
            world = twoSpheresScene();
            lookFrom = {13, 2, 3};
            lookAt = {0, 0, 0};
            vFov = 20.0;
            break;

        case 3:
            world = twoPerlinSpheres();
            lookFrom = {13, 2, 3};
            lookAt = {0, 0, 0};
            vFov = 20.0;
            break;

        default:
        case 4:
            world = earthScene();
            lookFrom = {13, 2, 3};
            lookAt = {0, 0, 0};
            vFov = 20.0;
            break;
    }

    Camera cam{lookFrom, lookAt, vup, vFov, aspectRatio, aperture, distToFocus, t0, t1};

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