#pragma once
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"

#include <memory>

class Box : public Hittable {
  public:
    Box(const Vec3& min, const Vec3& max, const std::shared_ptr<Material>& ptr)
        : min{min}, max{max} {
        sides.add(std::make_shared<XYRect>(min.x(), max.x(), min.y(), max.y(), max.z(), ptr));
        sides.add(std::make_shared<XYRect>(min.x(), max.x(), min.y(), max.y(), min.z(), ptr));

        sides.add(std::make_shared<XZRect>(min.x(), max.x(), min.z(), max.z(), max.y(), ptr));
        sides.add(std::make_shared<XZRect>(min.x(), max.x(), min.z(), max.z(), min.y(), ptr));

        sides.add(std::make_shared<YZRect>(min.y(), max.y(), min.z(), max.z(), max.x(), ptr));
        sides.add(std::make_shared<YZRect>(min.y(), max.y(), min.z(), max.z(), min.x(), ptr));
    }

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override {
        return sides.hit(r, tmin, tmax, rec);
    }

    bool boundingBox(double time0, double time1, AABB& outBox) const override {
        outBox = AABB(min, max);
        return true;
    }

  private:
    Vec3 min;
    Vec3 max;
    HittableList sides;
};

HittableList cornellBox() {
    using std::make_shared;
    using std::shared_ptr;
    HittableList world;

    auto red = make_shared<Lambertian>(Vec3{0.65, 0.05, 0.05});
    auto white = make_shared<Lambertian>(Vec3{0.73, 0.73, 0.73});
    auto green = make_shared<Lambertian>(Vec3{0.12, 0.45, 0.15});
    auto light = make_shared<DiffuseLight>(Vec3{15, 15, 15});

    world.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
    world.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
    world.add(make_shared<XZRect>(213, 343, 227, 332, 554, light));
    world.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
    world.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
    world.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

    shared_ptr<Hittable> box1 = make_shared<Box>(Vec3{0, 0, 0}, Vec3{165, 330, 165}, white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vec3{265, 0, 295});
    world.add(box1);

    shared_ptr<Hittable> box2 = make_shared<Box>(Vec3{0, 0, 0}, Vec3{165, 165, 165}, white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vec3{130, 0, 65});
    world.add(box2);


    return world;
}