#pragma once

#include "vec.h"
#include "rtweekend.h"

#include <algorithm>
#include <vector>

class PerlinNoise {
  public:
    static const int pointCount = 256;

  public:
    PerlinNoise() {
        randomVectors.resize(pointCount);
        for (auto& x : randomVectors) {
            x = gen.randomVec3OnUnitSphere();
        }

        permX = generatePermuation();
        permY = generatePermuation();
        permZ = generatePermuation();
    }

  public:
    double noise(const Vec3& p) const {
        auto u = p.x() - floor(p.x());
        auto v = p.y() - floor(p.y());
        auto w = p.z() - floor(p.z());

        auto i = static_cast<int>(floor(p.x())) & 255;
        auto j = static_cast<int>(floor(p.y())) & 255;
        auto k = static_cast<int>(floor(p.z())) & 255;
        Vec3 c[2][2][2];

        for (int di = 0; di < 2; ++di) {
            for (int dj = 0; dj < 2; ++dj) {
                for (int dk = 0; dk < 2; ++dk) {
                    c[di][dj][dk] = randomVectors[permX[(i + di) & 255] ^ permY[(j + dj) & 255] ^
                                                  permZ[(k + dk) & 255]];
                }
            }
        }

        return perlinInterpolation(c, u, v, w);
    }

    double turb(const Vec3& p, int depth = 7) const {
        auto sum = 0.0;
        auto temp = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; ++i) {
            sum += weight * noise(temp);
            weight *= 0.5;
            temp *= 2;
        }

        return std::abs(sum);
    }

  private:
    template <typename T>
    static void shuffle(std::vector<T>& v) {
        for (auto i = v.size() - 1; i > 0; --i) {
            auto j = gen.randomInt(0, i - 1);
            std::swap(v[j], v[i]);
        }
    }

    static std::vector<int> generatePermuation() {
        std::vector<int> res(pointCount);
        std::iota(res.begin(), res.end(), 0);
        shuffle(res);
        return res;
    }

    static double perlinInterpolation(Vec3 c[2][2][2], double u, double v, double w) {
        auto uu = u * u * (3 - 2 * u);
        auto vv = v * v * (3 - 2 * v);
        auto ww = w * w * (3 - 2 * w);
        double sum = 0.0;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    Vec3 weight(u - i, v - j, w - k);
                    sum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) *
                           (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weight);
                }
            }
        }
        return sum;
    }

    std::vector<Vec3> randomVectors;
    std::vector<int> permX;
    std::vector<int> permY;
    std::vector<int> permZ;
};