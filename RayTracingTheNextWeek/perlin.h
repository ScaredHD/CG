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
        randomValues.resize(pointCount);

        std::for_each(randomValues.begin(), randomValues.end(),
                      [](double& x) { x = gen.randomDouble(); });

        permX = generatePermuation();
        permY = generatePermuation();
        permZ = generatePermuation();
    }

  public:
    double noise(const Vec3& p) const {
        auto u = p.x() - floor(p.x());
        auto v = p.y() - floor(p.y());
        auto w = p.z() - floor(p.z());
        u = u * u * (3 - 2 * u);
        v = v * v * (3 - 2 * v);
        w = w * w * (3 - 2 * w);

        auto i = static_cast<int>(floor(p.x())) & 255;
        auto j = static_cast<int>(floor(p.y())) & 255;
        auto k = static_cast<int>(floor(p.z())) & 255;
        double c[2][2][2];

        for (int di = 0; di < 2; ++di) {
            for (int dj = 0; dj < 2; ++dj) {
                for (int dk = 0; dk < 2; ++dk) {
                    c[di][dj][dk] = randomValues[permX[(i + di) & 255] ^ permY[(j + dj) & 255] ^
                                                 permZ[(k + dk) & 255]];
                }
            }
        }

        return trilinear(c, u, v, w);
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

    static double trilinear(double c[2][2][2], double u, double v, double w) {
        double sum = 0.0;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    sum += (i * u + (1 - i) * (1 - u)) * (j * v + (1 - j) * (1 - v)) *
                           (k * w + (1 - k) * (1 - w)) * c[i][j][k];
                }
            }
        }
        return sum;
    }

    std::vector<double> randomValues;
    std::vector<int> permX;
    std::vector<int> permY;
    std::vector<int> permZ;
};