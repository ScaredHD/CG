#pragma once

#include "matrix.h"
#include "camera.h"

Mat4 translateTransformation(double x, double y, double z);

Mat4 translateTransformation(const Vec3& v);

Mat4 rotateX(double deg);

Mat4 rotateY(double deg);

Mat4 rotateZ(double deg);

Mat4 rotateTransformation(double x, double y, double z);

Mat4 rotateTransformation(const Vec3& axis, double deg);

Mat4 scaleTransformation(double scaleX, double scaleY, double scaleZ);

Mat4 viewTransformation(const Camera& cam);

Mat4 projectionTransformation(const Camera& cam);


