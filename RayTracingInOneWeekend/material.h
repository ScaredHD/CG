#pragma once

#include "rtweekend.h"
#include "hittable.h"

struct HitRecord;

struct Material {
    virtual bool scatter(const Ray& incident, const HitRecord& rec, Vec3& attenuation,
                         Ray& scattered) const = 0;
};

struct Lambertian : public Material {
    Lambertian(const Vec3& albedo) : albedo(albedo) {}

    virtual bool scatter(const Ray& incident, const HitRecord& rec, Vec3& attenuation,
                         Ray& scattered) const override {
        scattered.o = rec.p;
        scattered.d = rec.normal + gen.randomVec3OnUnitSphere();
        if (scattered.d.nearZero()) scattered.d = rec.normal;
        attenuation = albedo;
        return true;
    }

    Vec3 albedo;
};

struct Metal : public Material {
    Metal(const Vec3& albedo) : albedo(albedo) {}

    virtual bool scatter(const Ray& incident, const HitRecord& rec, Vec3& attenuation,
                         Ray& scattered) const override {
        auto d = incident.d;
        auto n = rec.normal;
        auto r = d - 2 * dot(d, n) * n;

        scattered.o = rec.p;
        scattered.d = r;
        attenuation = albedo;
        return dot(scattered.d, rec.normal) > 0;
    }

    Vec3 albedo;
};