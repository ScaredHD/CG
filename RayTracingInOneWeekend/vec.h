#pragma once

#include <cmath>
#include <array>

class Vec3 {
  public:
    Vec3() : Vec3(0, 0, 0) {}
    Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    const double& operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }

    Vec3& operator+=(const Vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vec3& operator/=(double t) { return *this *= 1 / t; }

    double length() const { return std::sqrt(lengthSquared()); }

    double lengthSquared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

  private:
    std::array<double, 3> e;
};

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return Vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return u + (-v);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline Vec3 operator*(double t, const Vec3& v) {
    return Vec3(v[0] * t, v[1] * t, v[2] * t);
}

inline Vec3 operator*(const Vec3& v, double t) {
    return t * v;
}

inline Vec3 operator/(const Vec3& v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vec3& u, const Vec3& v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
    return Vec3(u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]);
}

inline Vec3 normalized(const Vec3& v) {
    return v / v.length();
}