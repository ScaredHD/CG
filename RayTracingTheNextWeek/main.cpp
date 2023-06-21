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

HittableList randomScene() {
    HittableList world;
    auto groundMaterial = make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Vec3(0, -1000, 0), 1000, groundMaterial));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            auto dice = gen.randomDouble();
            Vec3 center(a + 0.9 * gen.randomDouble(), 0.2, b + 0.9 * gen.randomDouble());

            if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> mat;
                if (dice < 0.8) {
                    // diffuse
                    auto albedo = gen.randomVec3() * gen.randomVec3();
                    mat = make_shared<Lambertian>(albedo);
                } else if (dice < 0.95) {
                    // metal
                    auto albedo = gen.randomVec3(0.5, 1);
                    auto roughness = gen.randomDouble(0, 0.5);
                    mat = make_shared<Metal>(albedo, roughness);
                } else {
                    // glass
                    mat = make_shared<Dielectric>(1.5);
                }
                world.add(make_shared<Sphere>(center, 0.2, mat));
            }
        }
    }

    auto mat1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Vec3(0, 1, 0), 1.0, mat1));

    auto mat2 = make_shared<Lambertian>(Vec3(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, mat2));

    auto mat3 = make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Vec3(4, 1, 0), 1.0, mat3));

    return world;
}

int main() {
    // Image
    const double aspectRatio = 3.0 / 2.0;
    const int imageWidth = 1200;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 500;
    const int maxDepth = 50;

    // Camera
    Vec3 lookFrom(13, 2, 3);
    Vec3 lookAt(0, 0, 0);
    Vec3 vup(0, 1, 0);
    auto distToFocus = 10.0;
    auto aperture = 0.1;
    Camera cam(lookFrom, lookAt, vup, 20, aspectRatio, aperture, distToFocus);

    // Objects
    HittableList world = randomScene();

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