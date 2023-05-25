#pragma once
#include <array>
#include <cmath>
#include <numeric>
#include <ostream>
#include <algorithm>
#include <tuple>

#include "vec.h"

template <size_t L, typename T>
auto transposed(const std::array<std::array<T, L>, L>& m) {
    std::array<std::array<T, L>, L> res;
    for (size_t i = 0; i < L; ++i)
        for (size_t j = 0; j < L; ++j) res[i][j] = m[j][i];
    return res;
}

template <size_t L, typename T>
struct Matrix {
    Matrix() {}
    Matrix(const std::array<std::array<T, L>, L>& mat) : mat(mat) {}

    std::array<T, L>& operator[](int i) { return mat[i]; }
    const std::array<T, L>& operator[](int i) const { return mat[i]; }

    Matrix transposed() const;

    std::array<std::array<T, L>, L> mat;
};

template <size_t L, typename T>
Matrix<L, T> Matrix<L, T>::transposed() const {
    std::array<std::array<T, L>, L> res;
    for (size_t i = 0; i < L; ++i)
        for (size_t j = 0; j < L; ++j) res[i][j] = mat[j][i];
    return res;
}

// Matrix(L x L) + Matrix(L x L)
template <size_t L, typename T>
Matrix<L, T> operator+(const Matrix<L, T>& m, const Matrix<L, T>& n) {
    Matrix<L, T> res;
    std::transform(m.mat.begin(), m.mat.end(), n.mat.begin(), res.mat.begin(),
                   [](const std::array<T, L>& a, const std::array<T, L>& b) { return a + b; });
    return res;
}

// Matrix(L x L) * x
template <size_t L, typename T>
Matrix<L, T> operator*(const Matrix<L, T>& m, T x) {
    Matrix<L, T> res(m);
    std::for_each(m.begin(), m.end(), [x](std::array<T, L>& row) { row = row * x; });
    return res;
}

// Matrix(L x L) * Matrix(L x L)
template <size_t L, typename T>
Matrix<L, T> operator*(const Matrix<L, T>& m, const Matrix<L, T>& n) {
    const auto& [rows, cols] = std::make_tuple(m.mat, n.transposed().mat);
    Matrix<L, T> res;
    for (size_t i = 0; i < L; ++i) {
        for (size_t j = 0; j < L; ++j) {
            res[i][j] = dot(rows[i], cols[j]);
        }
    }
    return res;
}

// Matrix(L x L) * Vector(L x 1)
template <size_t L, typename T>
VectorX<L, T> operator*(const Matrix<L, T>& m, const VectorX<L, T>& v) {
    VectorX<L, T> res;
    std::transform(m.mat.begin(), m.mat.end(), res.arr.begin(),
                   [v](const VectorX<L, T>& a) { return dot(a, v); });
    return res;
}

using Mat2 = Matrix<2, double>;
using Mat3 = Matrix<3, double>;
using Mat4 = Matrix<4, double>;

Mat4 homogeneousMatrix(const Mat3& m);
Mat3 linearPart(const Mat4& m);