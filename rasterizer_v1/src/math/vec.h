#pragma once

#include <array>
#include <cmath>
#include <ostream>
#include <algorithm>
#include <numeric>

// array(L) + array(L)
template <size_t L, typename T>
std::array<T, L> operator+(const std::array<T, L>& a, const std::array<T, L>& b) {
    std::array<T, L> res;
    std::transform(a.begin(), a.end(), b.begin(), res.begin(), std::plus<>());
    return res;
}

// array(L) * c
template <size_t L, typename T>
std::array<T, L> operator*(const std::array<T, L>& v, T c) {
    std::array<T, L> res(v);
    std::for_each(res.begin(), res.end(), [c](T& x) { x *= c; });
    return res;
}

template <size_t L, typename T>
T dot(const std::array<T, L>& a, const std::array<T, L>& b) {
    std::array<T, L> prod;
    std::transform(a.begin(), a.end(), b.begin(), prod.begin(), std::multiplies<>());
    return std::accumulate(prod.begin(), prod.end(), T(0));
}

template <size_t L, typename T>
struct VectorX {
    VectorX() {}
    VectorX(const std::array<T, L>& v) : arr(v) {}
    VectorX(const VectorX& other) : arr(other.arr) {}

    VectorX operator-() { return VectorX(arr * (-1.0)); }
    VectorX operator-() const { return VectorX(arr * (-1.0)); }
    VectorX& operator+=(const VectorX& other) { return *this = *this + other; }
    VectorX& operator-=(const VectorX& other) { return *this = *this - other; }
    VectorX& operator*=(T x) { return *this = *this * x; }
    VectorX& operator/=(T x) { return *this = *this / x; }
    T& operator[](int i) { return arr[i]; }
    const T& operator[](int i) const { return arr[i]; }

    template <typename U>
    operator VectorX<L, U>() {
        std::array<U, L> u;
        std::transform(arr.begin(), arr.end(), u.begin(), [](const T& x) { return U(x); });
        return VectorX<L, U>(u);
    }

    T norm() const { return std::sqrt(dot(arr, arr)); }
    VectorX normalized() const { return *this / norm(); }

    std::array<T, L> arr;
};

template <size_t L, typename T>
VectorX<L, T> operator+(const VectorX<L, T>& a, const VectorX<L, T>& b) {
    return VectorX(a.arr + b.arr);
}

template <size_t L, typename T>
VectorX<L, T> operator-(const VectorX<L, T>& a, const VectorX<L, T>& b) {
    return a + b * (-1);
}

template <size_t L, typename T>
VectorX<L, T> operator*(const VectorX<L, T>& a, T x) {
    return VectorX(a.arr * x);
}

template <size_t L, typename T>
VectorX<L, T> operator*(T x, const VectorX<L, T>& a) {
    return a * x;
}

template <size_t L, typename T>
VectorX<L, T> operator/(const VectorX<L, T>& a, T x) {
    return a * (1 / x);
}

template <size_t L, typename T>
auto dot(const VectorX<L, T>& a, const VectorX<L, T>& b) {
    return dot(a.arr, b.arr);
}

template <typename T>
VectorX<3, T> cross(const VectorX<3, T>& a, const VectorX<3, T>& b) {
    const auto& [a0, a1, a2] = a.arr;
    const auto& [b0, b1, b2] = b.arr;
    return {{a1 * b2 - a2 * b1, a2 * b0 - a0 * b2, a0 * b1 - a1 * b0}};
}

using Vec2 = VectorX<2, double>;
using Vec3 = VectorX<3, double>;
using Vec4 = VectorX<4, double>;

template <size_t L, typename T>
std::ostream& operator<<(std::ostream& os, const VectorX<L, T>& v) {
    for (int i = 0; i < L; ++i) {
        os << v[i] << (i == L - 1 ? ", " : "");
    }
    return os;
}

Vec4 homogeneous(const Vec3& v);
Vec3 hnormalized(const Vec4& v);