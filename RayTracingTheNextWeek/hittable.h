#pragma once

#include <memory>
#include <utility>
#include <cmath>

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

    double u;
    double v;

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
        // Check if ray hits this sphere
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

        // Update hit record accordingly
        rec.t = t;
        rec.p = r.at(t);
        auto outwardNormal = (rec.p - center) / radius;
        rec.setFaceNormal(r, outwardNormal);
        getSphereUV(outwardNormal, rec.u, rec.v);
        rec.material = material;
        return true;
    }

    static void getSphereUV(const Vec3& p, double& outU, double& outV) {
        const auto& [x, y, z] = std::make_tuple(p.x(), p.y(), p.z());

        // Cartesian to spherical coordinates
        auto theta = std::acos(-y);         // polar angle
        auto phi = std::atan2(-z, x) + pi;  // azimuthal angle

        // Normalized spherical coordinates to get u-v coordinates
        outU = phi / (2 * pi);
        outV = theta / pi;
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

class XYRect : public Hittable {
  public:
    XYRect(double x0, double x1, double y0, double y1, double k, std::shared_ptr<Material> mat)
        : x0{x0}, x1{x1}, y0{y0}, y1{y1}, k{k}, material{std::move(mat)} {}

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override {
        auto t = (k - r.o.z()) / r.d.z();
        if (t < tmin || t > tmax) return false;

        auto x = r.at(t).x();
        auto y = r.at(t).y();
        if (x < x0 || x > x1 || y < y0 || y > y1) return false;

        rec.u = (x - x0) / (x1 - x0);
        rec.v = (y - y0) / (y1 - y0);
        rec.t = t;
        rec.p = r.at(t);
        rec.setFaceNormal(r, {0, 0, 1});
        rec.material = material;
        return true;
    }

    bool boundingBox(double time0, double time1, AABB& outBox) const override {
        outBox = AABB(Vec3{x0, y0, k - 0.0001}, Vec3{x1, y1, k + 0.0001});
        return true;
    }

  private:
    double x0;
    double x1;
    double y0;
    double y1;
    double k;
    std::shared_ptr<Material> material;
};

class XZRect : public Hittable {
  public:
    XZRect(double x0, double x1, double z0, double z1, double k, std::shared_ptr<Material> mat)
        : x0{x0}, x1{x1}, z0{z0}, z1{z1}, k{k}, material{std::move(mat)} {}

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override {
        auto t = (k - r.o.y()) / r.d.y();
        if (t < tmin || t > tmax) return false;

        auto x = r.at(t).x();
        auto z = r.at(t).z();
        if (x < x0 || x > x1 || z < z0 || z > z1) return false;

        rec.u = (x - x0) / (x1 - x0);
        rec.v = (z - z0) / (z1 - z0);
        rec.t = t;
        rec.p = r.at(t);
        rec.setFaceNormal(r, {0, 1, 0});
        rec.material = material;
        return true;
    }

    bool boundingBox(double time0, double time1, AABB& outBox) const override {
        outBox = AABB(Vec3{x0, k - 0.0001, z0}, Vec3{x1, k + 0.0001, z1});
        return true;
    }

  private:
    double x0;
    double x1;
    double z0;
    double z1;
    double k;
    std::shared_ptr<Material> material;
};

class YZRect : public Hittable {
  public:
    YZRect(double y0, double y1, double z0, double z1, double k, std::shared_ptr<Material> mat)
        : y0{y0}, y1{y1}, z0{z0}, z1{z1}, k{k}, material{std::move(mat)} {}

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override {
        auto t = (k - r.o.x()) / r.d.x();
        if (t < tmin || t > tmax) return false;

        auto y = r.at(t).y();
        auto z = r.at(t).z();
        if (y < y0 || y > y1 || z < z0 || z > z1) return false;

        rec.u = (y - y0) / (y1 - y0);
        rec.v = (z - z0) / (z1 - z0);
        rec.t = t;
        rec.p = r.at(t);
        rec.setFaceNormal(r, {1, 0, 0});
        rec.material = material;
        return true;
    }

    bool boundingBox(double time0, double time1, AABB& outBox) const override {
        outBox = AABB(Vec3{k - 0.0001, y0, z0}, Vec3{k + 0.0001, y0, z1});
        return true;
    }

  private:
    double y0;
    double y1;
    double z0;
    double z1;
    double k;
    std::shared_ptr<Material> material;
};

