#include <iostream>

#include "rtweekend.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "camera.h"
#include "box.h"
#include "constant_medium.h"
#include "bvh.h"

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

HittableList final_scene() {
    HittableList boxes1;
    auto ground = make_shared<Lambertian>(Vec3(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = gen.randomDouble(1, 101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<Box>(Vec3(x0, y0, z0), Vec3(x1, y1, z1), ground));
        }
    }

    HittableList objects;

    objects.add(make_shared<BvhNode>(boxes1, 0, 1));

    auto light = make_shared<DiffuseLight>(Vec3(7, 7, 7));
    objects.add(make_shared<XZRect>(123, 423, 147, 412, 554, light));

    auto center1 = Vec3(400, 400, 200);
    auto center2 = center1 + Vec3(30, 0, 0);
    auto moving_sphere_material = make_shared<Lambertian>(Vec3(0.7, 0.3, 0.1));
    objects.add(make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<Sphere>(Vec3(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    objects.add(make_shared<Sphere>(Vec3(0, 150, 145), 50,
                                    make_shared<Metal>(Vec3(0.8, 0.8, 0.9), 1.0)));

    auto boundary = make_shared<Sphere>(Vec3(360, 150, 145), 70, make_shared<Dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<ConstantMedium>(boundary, 0.2, Vec3(0.2, 0.4, 0.9)));
    boundary = make_shared<Sphere>(Vec3(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
    objects.add(make_shared<ConstantMedium>(boundary, .0001, Vec3(1, 1, 1)));

    auto emat = make_shared<Lambertian>(make_shared<ImageTexture>("earthmap.jpg"));
    objects.add(make_shared<Sphere>(Vec3(400, 200, 400), 100, emat));
    auto pertext = make_shared<NoiseTexture>(0.1);
    objects.add(make_shared<Sphere>(Vec3(220, 280, 300), 80, make_shared<Lambertian>(pertext)));

    HittableList boxes2;
    auto white = make_shared<Lambertian>(Vec3(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<Sphere>(gen.randomVec3(0, 165), 10, white));
    }

    objects.add(make_shared<Translate>(
        make_shared<RotateY>(make_shared<BvhNode>(boxes2, 0.0, 1.0), 15), Vec3(-100, 270, 395)));

    return objects;
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

        case 6:
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

        case 7:
            world = cornellSmokeScene();
            aspectRatio = 1.0;
            imageWidth = 600;
            imageHeight = static_cast<int>(imageWidth / aspectRatio);
            samplesPerPixel = 200;
            backgroundColor = {0, 0, 0};
            lookFrom = {278, 278, -800};
            lookAt = {278, 278, 0};
            vFov = 40.0;
            
        default:
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