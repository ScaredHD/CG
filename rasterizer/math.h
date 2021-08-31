#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <Eigen/Core>

const double pi = acos(-1);

const double inf = std::numeric_limits<double>::infinity();

double clamp(double x, double low, double high);

Eigen::Vector3d cross(const Eigen::Vector3d& u, const Eigen::Vector3d& v);

Eigen::Vector3d vecminus(const Eigen::Vector3d& u, const Eigen::Vector3d& v);
