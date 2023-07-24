#pragma once

#include <memory>
#include <cmath>

#include "vec.h"
#include "perlin.h"

class Texture {
  public:
    virtual Vec3 value(double u, double v, const Vec3& p) const = 0;
};

class SolidColor : public Texture {
  public:
    SolidColor(const Vec3& color = Vec3{}) : colorValue{color} {}

    SolidColor(double r, double g, double b) : colorValue{r, g, b} {}

    Vec3 value(double u, double v, const Vec3& p) const override { return colorValue; }

  private:
    Vec3 colorValue;
};

class CheckerTexture : public Texture {
  public:
    CheckerTexture(std::shared_ptr<Texture> odd, std::shared_ptr<Texture> even)
        : odd{std::move(odd)}, even{std::move(even)} {}

    CheckerTexture(const Vec3& color1, const Vec3& color2)
        : CheckerTexture{std::make_shared<SolidColor>(color1),
                         std::make_shared<SolidColor>(color2)} {}

    Vec3 value(double u, double v, const Vec3& p) const override {
        using std::sin;
        auto s = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        return (s < 0 ? odd : even)->value(u, v, p);
    }

  private:
    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;
};

class NoiseTexture : public Texture {
  public:
    NoiseTexture() = default;
    NoiseTexture(double scale) : scale{scale} {}

    Vec3 value(double u, double v, const Vec3& p) const override {
        // return Vec3{1, 1, 1} * (1.0 + noise.noise(scale * p)) * 0.5;
        // return Vec3{1, 1, 1} * noise.turb(scale * p);
        return Vec3{1, 1, 1} * 0.5 * (1 + std::sin(scale * p.z() + 10 * noise.turb(p)));
    }

  private:
    PerlinNoise noise;
    double scale{1.0};
};