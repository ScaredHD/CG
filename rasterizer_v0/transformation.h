#pragma once

#include <Eigen/Eigen>
#include "math.h"

namespace matrix {

	Eigen::Matrix4d translation(const double& x, const double y, const double z);
	Eigen::Matrix4d translation(Eigen::Vector3d v);
	Eigen::Matrix4d rotationX(const double& deg);
	Eigen::Matrix4d rotationY(const double& deg);
	Eigen::Matrix4d rotationZ(const double& deg);
	Eigen::Matrix4d rotation(const Eigen::Vector3d& n, double deg);
	Eigen::Vector3d rotation(const Eigen::Vector3d& v, const Eigen::Vector3d axis, double deg);

}; // namespace matrix

