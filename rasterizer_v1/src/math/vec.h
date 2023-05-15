#pragma once

#include <array>
#include <cmath>
#include <ostream>

template <size_t Len, typename T = double>
class VectorX {
  public:
    VectorX() : v(Len, 0) {}
    VectorX(const std::array<T, Len>& v) : v(v) {}

    T& operator[](size_t i) { return v[i]; }
    const T& operator[](size_t i) const { return v[i]; }
    T operator-() const;
    T& operator+=(const VectorX<Len, T>& v);
    const T& operator+=(const VectorX<Len, T>& v) const;
    T& operator-=(const VectorX<Len, T>& v);
    const T& operator-=(const VectorX<Len, T>& v) const;
    T& operator*=(T x);
    const T& operator*=(T x) const;
    T& operator/=(T x);
    const T& operator/=(T x) const;

    // common vector operations
    VectorX<Len, T> normalized() const;
    T norm() const;

  private:
    std::array<T, Len> v;
};

template <size_t Len, typename T>
std::ostream& operator<<(std::ostream& os, const VectorX<Len, T>& v) {
    for (size_t i = 0; i < Len; ++i) {
        os << v[i] << (i != Len - 1 ? ", " : "");
    }
    return os;
}

// arithmetics between Vectors and numbers
template <size_t Len, typename T>
auto operator+(const VectorX<Len, T>& u, const VectorX<Len, T>& v) {
    VectorX<Len, T> res(u);
    for (size_t i = 0; i < Len; ++i) res[i] += v[i];
    return res;
}

template <size_t Len, typename T>
T VectorX<Len, T>::operator-() const {
    VectorX<Len, T> res(v);
    for (size_t i = 0; i < Len; ++i) res[i] = -res[i];
    return res;
}

template <size_t Len, typename T>
auto operator-(const VectorX<Len, T>& u, const VectorX<Len, T>& v) {
    return u + (-v);
}

template <size_t Len, typename T>
auto operator*(T x, const VectorX<Len, T>& v) {
    VectorX<Len, T> res(v);
    for (size_t i = 0; i < Len; ++i) res[i] *= x;
    return res;
}

template <size_t Len, typename T>
auto operator*(const VectorX<Len, T>& v, T x) {
    VectorX<Len, T> res(v);
    for (size_t i = 0; i < Len; ++i) res[i] *= x;
    return res;
}

template <size_t Len, typename T>
auto operator/(const VectorX<Len, T>& v, T x) {
    return v * (1 / x);
}

template <size_t Len, typename T>
T& VectorX<Len, T>::operator+=(const VectorX<Len, T>& v) {
    return *this = (*this) + v;
}

template <size_t Len, typename T>
const T& VectorX<Len, T>::operator+=(const VectorX<Len, T>& v) const {
    return *this = (*this) + v;
}

template <size_t Len, typename T>
T& VectorX<Len, T>::operator*=(T x) {
    return *this = (*this) * x;
}

template <size_t Len, typename T>
const T& VectorX<Len, T>::operator*=(T x) const {
    return *this = (*this) * x;
}

template <size_t Len, typename T>
T& VectorX<Len, T>::operator/=(T x) {
    return *this = (*this) / x;
}

template <size_t Len, typename T>
const T& VectorX<Len, T>::operator/=(T x) const {
    return *this = (*this) / x;
}

template <size_t Len, typename T>
T dot(const VectorX<Len, T>& u, const VectorX<Len, T>& v) {
    T res = T(0);
    for (size_t i = 0; i < Len; ++i) {
        res += u[i] * v[i];
    }
    return res;
}

// common vector operations
template <size_t Len, typename T>
T VectorX<Len, T>::norm() const {
    T sumOfSquares = T(0);
    for (size_t i = 0; i < Len; ++i) {
        sumOfSquares += v[i] * v[i];
    }
    return std::sqrt(sumOfSquares);
}

template <size_t Len, typename T>
VectorX<Len, T> VectorX<Len, T>::normalized() const {
    return (*this) / norm();
}

// Vector2/3/4
template <typename T = double>
class Vector2 : public VectorX<2, T> {
  public:
    Vector2() : VectorX<2, T>() {}
    Vector2(T x, T y) : VectorX<2, T>({x, y}) {}

    T& x() { return (*this)[0]; }
    T& y() { return (*this)[1]; }
    const T& x() const { return (*this)[0]; }
    const T& y() const { return (*this)[1]; }
};

template <typename T = double>
class Vector3 : public VectorX<3, T> {
  public:
    Vector3() : VectorX<3, T>() {}
    Vector3(T x, T y, T z) : VectorX<3, T>({x, y, z}) {}
    Vector3(const VectorX<3, T>& other) : VectorX<3, T>(other) {}

    T& x() { return (*this)[0]; }
    T& y() { return (*this)[1]; }
    T& z() { return (*this)[2]; }
    const T& x() const { return (*this)[0]; }
    const T& y() const { return (*this)[1]; }
    const T& z() const { return (*this)[2]; }
};

template <typename T = double>
class Vector4 : public VectorX<4, T> {
  public:
    Vector4() : VectorX<4, T>() {}
    Vector4(T x, T y, T z, T w) : VectorX<4, T>({x, y, z, w}) {}

    T& x() { return (*this)[0]; }
    T& y() { return (*this)[1]; }
    T& z() { return (*this)[2]; }
    T& w() { return (*this)[3]; }
    const T& x() const { return (*this)[0]; }
    const T& y() const { return (*this)[1]; }
    const T& z() const { return (*this)[2]; }
    const T& w() const { return (*this)[3]; }
};

using Vec2 = Vector2<double>;
using Vec3 = Vector3<double>;
using Vec4 = Vector4<double>;

template <typename T>
Vec3 cross(const VectorX<3, T>& u, const VectorX<3, T>& v) {
    Vec3 res;
    res.x() = u.y() * v.z() - u.z() * v.y();
    res.y() = u.z() * v.x() - u.x() * v.z();
    res.z() = u.x() * v.y() - u.y() * v.x();
    return res;
}