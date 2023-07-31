#pragma once

#include "rtweekend.h"
#include "hittable.h"
#include "material.h"
#include "texture.h"
#include "hittable_list.h"
#include "box.h"

class ConstantMedium : public Hittable {
  public:
    ConstantMedium(const std::shared_ptr<Hittable>& b, double d, const std::shared_ptr<Texture>& a)
        : boundary{b}, phaseFunction{std::make_shared<Isotropic>(a)}, negInvDensity{-1 / d} {}

    ConstantMedium(const std::shared_ptr<Hittable>& b, double d, const Vec3& color)
        : boundary{b}, phaseFunction{std::make_shared<Isotropic>(color)}, negInvDensity{-1 / d} {}

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override {
        HitRecord rec1;
        if (!boundary->hit(r, -infinity, infinity, rec1)) return false;

        HitRecord rec2;
        if (!boundary->hit(r, rec1.t + 0.0001, infinity, rec2)) return false;

        if (rec1.t < tmin) rec1.t = tmin;
        if (rec2.t > tmax) rec2.t = tmax;
        if (rec1.t >= rec2.t) return false;
        if (rec1.t < 0) rec1.t = 0;

        auto rayLength = r.d.length();
        auto distInside = (rec2.t - rec1.t) * rayLength;
        auto hitDist = negInvDensity * std::log(gen.randomDouble());

        if (hitDist > distInside) return false;

        rec.t = rec1.t + hitDist / rayLength;
        rec.p = r.at(rec.t);

        rec.normal = {1, 0, 0};
        rec.front = true;
        rec.material = phaseFunction;

        return true;
    }

    bool boundingBox(double time0, double time1, AABB& outBox) const override {
        return boundary->boundingBox(time0, time1, outBox);
    }

  private:
    std::shared_ptr<Hittable> boundary;
    std::shared_ptr<Material> phaseFunction;
    double negInvDensity;
};

HittableList cornellSmokeScene() {
    using std::make_shared;
    HittableList world;

    auto red = make_shared<Lambertian>(Vec3{0.65, 0.05, 0.05});
    auto white = make_shared<Lambertian>(Vec3{0.73, 0.73, 0.73});
    auto green = make_shared<Lambertian>(Vec3{0.12, 0.45, 0.15});
    auto light = make_shared<DiffuseLight>(Vec3{7, 7, 7});

    world.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
    world.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
    world.add(make_shared<XZRect>(113, 443, 127, 432, 554, light));
    world.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
    world.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
    world.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

    std::shared_ptr<Hittable> box1 = make_shared<Box>(Vec3{0, 0, 0}, Vec3{165, 330, 165}, white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vec3{265, 0, 295});
    world.add(make_shared<ConstantMedium>(box1, 0.01, Vec3{0, 0, 0}));

    std::shared_ptr<Hittable> box2 = make_shared<Box>(Vec3{0, 0, 0}, Vec3{165, 165, 165}, white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vec3{130, 0, 65});
    world.add(make_shared<ConstantMedium>(box2, 0.01, Vec3{1, 1, 1}));


    return world;
}
