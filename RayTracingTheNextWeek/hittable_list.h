#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "hittable.h"
#include "texture.h"
#include "material.h"

class HittableList : public Hittable {
  public:
    HittableList() = default;
    HittableList(const std::shared_ptr<Hittable>& object) { add(object); }

    void clear() { objects.clear(); }
    void add(const std::shared_ptr<Hittable>& object) { objects.push_back(object); }

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override;
    bool boundingBox(double time0, double time1, AABB& outputBox) const override;

    std::vector<std::shared_ptr<Hittable>> objects;
};

inline bool HittableList::hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const {
    HitRecord tempRec;
    bool hitAnything = false;

    for (const auto& p : objects) {
        if (p->hit(r, tmin, tmax, tempRec)) {
            hitAnything = true;
            tmax = tempRec.t;
            rec = tempRec;
        }
    }

    return hitAnything;
}

inline bool HittableList::boundingBox(double time0, double time1, AABB& outputBox) const {
    bool first = true;
    for (const auto& p : objects) {
        if (AABB box; p->boundingBox(time0, time1, box)) {
            outputBox = first ? box : surroundingBox(box, outputBox);
            first = false;
        }
    }
    return true;
}

HittableList randomScene() {
    using std::make_shared;
    using std::shared_ptr;
    HittableList world;

    auto checker = make_shared<CheckerTexture>(Vec3{0.2, 0.3, 0.1}, Vec3{0.9, 0.9, 0.9});
    auto groundMaterial = make_shared<Lambertian>(checker);
    world.add(make_shared<Sphere>(Vec3{0, -1000, 0}, 1000, groundMaterial));

    for (int a = -4; a < 4; ++a) {
        for (int b = -4; b < 4; ++b) {
            auto dice{gen.randomDouble()};
            Vec3 center{a + 0.9 * gen.randomDouble(), 0.2, b + 0.9 * gen.randomDouble()};
            Vec3 center2{center + Vec3{0, gen.randomDouble(0, 0.5), 0}};

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
                world.add(make_shared<MovingSphere>(center, center2, 0.0, 1.0, 0.2, mat));
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

HittableList twoSpheresScene() {
    using std::make_shared;
    HittableList world;

    auto checker = std::make_shared<CheckerTexture>(Vec3{0.2, 0.3, 0.1}, Vec3{0.9, 0.9, 0.9});

    world.add(make_shared<Sphere>(Vec3{0, -10, 0}, 10, make_shared<Lambertian>(checker)));
    world.add(make_shared<Sphere>(Vec3{0, 10, 0}, 10, make_shared<Lambertian>(checker)));
    return world;
}

HittableList twoPerlinSpheres() {
    using std::make_shared;
    HittableList world;

    auto noiseTexture = std::make_shared<NoiseTexture>(4);

    world.add(make_shared<Sphere>(Vec3{0, -1000, 0}, 1000, make_shared<Lambertian>(noiseTexture)));
    world.add(make_shared<Sphere>(Vec3{0, 2, 0}, 2, make_shared<Lambertian>(noiseTexture)));
    return world;
}

HittableList earthScene() {
    using std::make_shared;
    auto earthTexture = make_shared<ImageTexture>("earthmap.jpg");
    auto earthSurface = make_shared<Lambertian>(earthTexture);
    auto globe = make_shared<Sphere>(Vec3{0, 0, 0}, 2, earthSurface);
    return {globe};
}