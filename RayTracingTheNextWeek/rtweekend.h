#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "ray.h"
#include "vec.h"

constexpr double infinity = std::numeric_limits<double>::infinity();
const double pi = std::acos(-1.0);

template <typename T>
T lerp(const T& a, const T& b, double t) {
    return (1 - t) * a + t * b;
}

inline double toRadian(double deg) {
    return deg * pi / 180.0;
}

template <typename T>
T clamp(const T& x, const T& min, const T& max) {
    return x < min ? min : (x > max ? max : x);
}

struct RandomGenerator {
    std::random_device rd;
    std::mt19937 gen = std::mt19937(rd());

    int randomInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(gen);
    }

    double randomDouble(double min = 0.0, double maxExcluded = 1.0) {
        std::uniform_real_distribution<double> dist(min, maxExcluded);
        return dist(gen);
    }

    Vec3 randomVec3(double a = 0.0, double bExcluded = 1.0) {
        return {randomDouble(a, bExcluded), randomDouble(a, bExcluded), randomDouble(a, bExcluded)};
    }

    Vec3 randomVec3InUnitSphere() {
        while (true) {
            auto v = randomVec3(-1.0, 1.0);
            if (v.lengthSquared() >= 1) continue;
            return v;
        }
    }

    Vec3 randomVec3OnUnitSphere() { return normalized(randomVec3InUnitSphere()); }

    Vec3 randomInUnitDisk() {
        while (true) {
            auto p = Vec3(randomDouble(), randomDouble(), 0.0);
            if (p.length() >= 1) continue;
            return p;
        }
    }
};

static RandomGenerator gen;
