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
    Metal(const Vec3& albedo, double roughness = 0.0)
        : albedo(albedo), roughness(clamp(roughness, 0.0, 1.0)) {}

    virtual bool scatter(const Ray& incident, const HitRecord& rec, Vec3& attenuation,
                         Ray& scattered) const override {
        auto d = incident.d;
        auto n = rec.normal;
        auto r = d - 2 * dot(d, n) * n;

        scattered.o = rec.p;
        scattered.d = r + roughness * gen.randomVec3OnUnitSphere();
        attenuation = albedo;
        return dot(scattered.d, rec.normal) > 0;
    }

    Vec3 albedo;
    double roughness;
};

struct Dielectric : public Material {
    Dielectric(double refractiveIndex) : refractiveIndex(refractiveIndex) {}

    virtual bool scatter(const Ray& incident, const HitRecord& rec, Vec3& attenuation,
                         Ray& scattered) const override {
        double airIndex = 1.0;
        double relativeIndex = rec.front ? airIndex / refractiveIndex : refractiveIndex / airIndex;

        scattered.o = rec.p;
        scattered.d = refract(incident.d, rec.normal, relativeIndex);
        attenuation = Vec3(1, 1, 1);
        return true;
    }

    Vec3 refract(const Vec3& incidentDirection, const Vec3& normal, double relativeIndex) const {
        const auto& I = normalized(incidentDirection);
        const auto& N = normalized(normal);
        double IdotN = std::min(dot(I, N), 1.0);
        auto tangentPart = relativeIndex * (I - IdotN * N);
        auto normalPart = -N * std::sqrt(1 - relativeIndex * relativeIndex * (1 - IdotN * IdotN));
        return tangentPart + normalPart;
    }

    double refractiveIndex;
};