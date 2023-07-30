#include <iostream>

#include "rtweekend.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "camera.h"
#include "box.h"

using namespace std;

Vec3 rayColor(const Ray& r, const Vec3& backgroundColor, const HittableList& world, int maxDepth) {
    if (maxDepth <= 0) return {0, 0, 0};

    HitRecord rec;
    if (!world.hit(r, 0.001, infinity, rec)) return backgroundColor;

    Ray scattered;
    Vec3 attenuation;
    Vec3 emitted = rec.material->emitted(rec.u, rec.v, rec.p);
    if (rec.material->scatter(r, rec, attenuation, scattered)) {
        return emitted + attenuation * rayColor(scattered, backgroundColor, world, maxDepth - 1);
    } else {
        return emitted;
    }
}

int main() {
    // Image
    double aspectRatio = 16.0 / 9.0;
    int imageWidth = 400;
    int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    int samplesPerPixel = 32;
    int maxDepth = 32;

    // Objects
    HittableList world;
    Vec3 backgroundColor = {0, 0, 0};

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
            backgroundColor = {0.7, 0.8, 1.0};
            break;

        case 2:
            world = twoSpheresScene();
            lookFrom = {13, 2, 3};
            lookAt = {0, 0, 0};
            vFov = 20.0;
            backgroundColor = {0.7, 0.8, 1.0};
            break;

        case 3:
            world = twoPerlinSpheres();
            lookFrom = {13, 2, 3};
            lookAt = {0, 0, 0};
            vFov = 20.0;
            backgroundColor = {0.7, 0.8, 1.0};
            break;

        case 4:
            world = earthScene();
            lookFrom = {13, 2, 3};
            lookAt = {0, 0, 0};
            vFov = 20.0;
            backgroundColor = {0.7, 0.8, 1.0};
            break;

        case 5:
            world = simpleLightScene();
            lookFrom = {26, 3, 6};
            lookAt = {0, 2, 0};
            samplesPerPixel = 200;
            vFov = 20.0;
            backgroundColor = {0.0, 0.0, 0.0};
            break;

        default:
            world = cornellBox();
            aspectRatio = 1.0;
            imageWidth = 600;
            imageHeight = static_cast<int>(imageWidth / aspectRatio);
            samplesPerPixel = 200;
            backgroundColor = {0, 0, 0};
            lookFrom = {278, 278, -800};
            lookAt = {278, 278, 0};
            vFov = 40.0;
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
                color += rayColor(r, backgroundColor, world, maxDepth);
            }

            writeColor(cout, color, samplesPerPixel, true);
        }
    }
    cerr << "\nDone.\n";
}