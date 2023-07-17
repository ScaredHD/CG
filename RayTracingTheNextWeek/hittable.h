#pragma once

#include <memory>
#include <utility>

#include "vec.h"
#include "ray.h"
#include "rtweekend.h"
#include "aabb.h"

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
    virtual bool boundingBox(double time0, double time1, AABB& outBox) const = 0;
};


struct Sphere : public Hittable {
    Sphere(const Vec3& center, double radius, const std::shared_ptr<Material>& material)
        : center{center}, radius{radius}, material{material} {}

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override {
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

    bool boundingBox(double time0, double time1, AABB& outBox) const override {
        const auto& v = Vec3(radius, radius, radius);
        outBox = {center + v, center - v};
        return true;
    }

    Vec3 center;
    double radius;
    std::shared_ptr<Material> material;
};

class MovingSphere : public Hittable {
  public:
    MovingSphere(const Vec3& c0, const Vec3& c1, double t0, double t1, double radius,
                 const std::shared_ptr<Material>& material)
        : c0{c0}, c1{c1}, t0{t0}, t1{t1}, radius{radius}, material{material} {}

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override {
        auto oc = r.o - centerAtTime(r.time);
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
        auto outwardNormal = (rec.p - centerAtTime(r.time)) / radius;
        rec.setFaceNormal(r, outwardNormal);
        rec.material = material;
        return true;
    }

    Vec3 centerAtTime(double t) const { return lerp(c0, c1, (t - t0) / (t1 - t0)); }

    bool boundingBox(double time0, double time1, AABB& outBox) const override {
        const auto& v = Vec3(radius, radius, radius);
        const auto& c0 = centerAtTime(time0);
        const auto& c1 = centerAtTime(time1);
        const auto& box0 = AABB(c0 + v, c0 - v);
        const auto& box1 = AABB(c1 + v, c1 - v);
        outBox = surroundingBox(box0, box1);
        return true;
    }

  private:
    Vec3 c0;
    Vec3 c1;
    double t0;
    double t1;
    double radius;
    std::shared_ptr<Material> material;
};