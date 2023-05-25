#pragma once

#include "matrix.h"

Mat4 translateTransform(double x, double y, double z);

Mat4 translateTransform(const Vec3& v);

Mat4 rotateX(double deg);

Mat4 rotateY(double deg);

Mat4 rotateZ(double deg);

Mat4 rotateTransform(double x, double y, double z);

Mat4 rotateTransform(const Vec3& axis, double deg);

Mat4 scaleTransform(double scaleX, double scaleY, double scaleZ);

