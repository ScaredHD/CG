#include "matrix.h"

Mat4 homogeneousMatrix(const Mat3& m) {
    // clang-format off
    return {{{
        {m[0][0], m[0][1], m[0][2], 0.},
        {m[1][0], m[1][1], m[1][2], 0.},
        {m[2][0], m[2][1], m[2][2], 0.},
        {   0.,       0.,       0., 1.},
    }}};
    // clang-fromat on
}

Mat3 linearPart(const Mat4& m) {
    // clang-format off
    return {{{
        {m[0][0], m[0][1], m[0][2]},
        {m[1][0], m[1][1], m[1][2]},
        {m[2][0], m[2][1], m[2][2]},
    }}};
    // clang-fromat on    
}
