#pragma once

#include "rtweekend.h"
#include "hittable.h"

struct HitRecord;

static Vec3 reflect(const Vec3& incident, const Vec3& normal);
static Vec3 refract(const Vec3& incidentDirection, const Vec3& normal, double relativeIndex);

struct Material {
    virtual bool scatter(const Ray& incident, const HitRecord& rec, Vec3& attenuation,
                         Ray& scattered) const = 0;
};

struct Lambertian : public Material {
    Lambertian(const Vec3& albedo) : albedo(albedo) {}

    bool scatter(const Ray& incident, const HitRecord& rec, Vec3& attenuation,
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

    bool scatter(const Ray& incident, const HitRecord& rec, Vec3& attenuation,
                 Ray& scattered) const override {
        scattered.o = rec.p;
        scattered.d = reflect(incident.d, rec.normal) + roughness * gen.randomVec3OnUnitSphere();
        attenuation = albedo;
        return dot(scattered.d, rec.normal) > 0;
    }

    Vec3 albedo;
    double roughness;
};

struct Dielectric : public Material {
    Dielectric(double refractiveIndex) : refractiveIndex(refractiveIndex) {}

    bool scatter(const Ray& incident, const HitRecord& rec, Vec3& attenuation,
                 Ray& scattered) const override {
        double airIndex = 1.0;
        double relativeIndex = rec.front ? airIndex / refractiveIndex : refractiveIndex / airIndex;

        const auto& I = normalized(incident.d);
        const auto& N = normalized(rec.normal);
        double costheta = std::min(dot(-I, N), 1.0);
        double sintheta = std::sqrt(1 - costheta * costheta);

        scattered.o = rec.p;
        bool totalReflection = relativeIndex * sintheta > 1.0;
        scattered.d = totalReflection || reflectance(costheta, relativeIndex) > gen.randomDouble()
                          ? reflect(I, N)
                          : refract(I, N, relativeIndex);
        attenuation = Vec3(1, 1, 1);
        return true;
    }

    static double reflectance(double cosine, double refractiveIndex) {
        auto r0 = (1 - refractiveIndex) / (1 + refractiveIndex);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow(1 - cosine, 5);
    }

    double refractiveIndex;
};

static Vec3 reflect(const Vec3& incident, const Vec3& normal) {
    const auto& d = normalized(incident);
    const auto& n = normalized(normal);
    return d - 2 * dot(d, n) * n;
}

static Vec3 refract(const Vec3& incidentDirection, const Vec3& normal, double relativeIndex) {
    const auto& I = normalized(incidentDirection);
    const auto& N = normalized(normal);
    double IdotN = std::min(dot(I, N), 1.0);
    auto tangentPart = relativeIndex * (I - IdotN * N);
    auto normalPart = -N * std::sqrt(1 - relativeIndex * relativeIndex * (1 - IdotN * IdotN));
    return tangentPart + normalPart;
}