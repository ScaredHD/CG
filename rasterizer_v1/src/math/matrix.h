#pragma once
#include <array>
#include <cmath>
#include <ostream>

#include "vec.h"

template <size_t L, typename T>
void assignTransposed(const std::array<std::array<T, L>, L> mat,
                      const std::array<std::array<T, L>, L>& other) {
    for (size_t row = 0; row < L; ++row)
        for (size_t col = 0; col < L; ++col) mat[row][col] = other[col][row];
}

template <size_t L, typename T = double>
class MatrixX {
  public:
    MatrixX() {}
    MatrixX(const std::array<std::array<T, L>, L>& mat);

    MatrixX& operator=(const MatrixX& other);
    MatrixX& operator=(const std::array<std::array<T, L>, L>& other);

    std::array<T, L>& operator[](size_t i) { return mat[i]; }
    const std::array<T, L>& operator[](size_t i) const { return mat[i]; }

    VectorX<L, T> row(int r) const;
    VectorX<L, T> col(int c) const;

  private:
    std::array<std::array<T, L>, L> mat;
    std::array<std::array<T, L>, L> matTransposed;
};

template <size_t L, typename T>
MatrixX<L, T>::MatrixX(const std::array<std::array<T, L>, L>& mat) : mat(mat) {
    assignTransposed(matTransposed, mat);
}

template <size_t L, typename T>
MatrixX<L, T>& MatrixX<L, T>::operator=(const MatrixX& other) {
    mat = other.mat;
    matTransposed = other.matTransposed;
}

template <size_t L, typename T>
MatrixX<L, T>& MatrixX<L, T>::operator=(const std::array<std::array<T, L>, L>& other) {
    mat = other;
    assignTransposed(matTransposed, other);
}

template <size_t L, typename T>
VectorX<L, T> MatrixX<L, T>::row(int r) const {
    return mat[r];
}

template <size_t L, typename T>
VectorX<L, T> MatrixX<L, T>::col(int c) const {
    return matTransposed[c];
}

using Mat2 = MatrixX<2, double>;
using Mat3 = MatrixX<3, double>;
using Mat4 = MatrixX<4, double>;

template <size_t L, typename T>
MatrixX<L, T> operator*(const MatrixX<L, T>& m, const MatrixX<L, T>& n) {
}

template <size_t L, typename T>
VectorX<L, T> operator*(const MatrixX<L, T>& m, const VectorX<L, T>& v) {
    VectorX<L, T> res;
    for (size_t i = 0; i < L; ++i) {
        res[i] = dot(m.row(i), v);
    }
    return res;
}

template <size_t L, typename T>
auto operator*(const MatrixX<L, T>& m, T x) {
    MatrixX<L, T> res(m);
    for (auto& row : res) {
        for (auto& elem : row) {
            elem *= x;
        }
    }
    return res;
}

template <size_t L, typename T>
auto operator*(T x, const MatrixX<L, T>& m) {
    return m * x;
}
