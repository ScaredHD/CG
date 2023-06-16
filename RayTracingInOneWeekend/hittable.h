#pragma once

#include <memory>

#include "vec.h"
#include "ray.h"

struct Material;

struct HitRecord {
    Vec3 p;
    Vec3 normal;
    double t;
    bool front;

    std::shared_ptr<Material> material;

    void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
        front = dot(r.d, outwardNormal) < 0;
        normal = front ? outwardNormal : -outwardNormal;
    }
};

struct Hittable {
    virtual bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const = 0;
};

struct Sphere : public Hittable {
    Sphere(const Vec3& center, double radius, std::shared_ptr<Material> material)
        : center(center), radius(radius), material(material) {}

    virtual bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const {
        auto oc = r.o - center;
        auto a = r.d.lengthSquared();
        auto h = dot(r.d, oc);
        auto c = oc.lengthSquared() - radius * radius;
        auto discriminant = h * h - a * c;
        if (discriminant < 0.0) {
            return false;
        }

        auto t = -(h + std::sqrt(discriminant)) / a;
        if (t < tmin || t > tmax) {
            t = (-h + std::sqrt(discriminant)) / a;
            if (t < tmin || t > tmax) return false;
        }

        rec.t = t;
        rec.p = r.at(t);
        auto outwardNormal = (rec.p - center) / radius;
        rec.setFaceNormal(r, outwardNormal);
        rec.material = material;
        return true;
    }

    Vec3 center;
    double radius;
    std::shared_ptr<Material> material;
};