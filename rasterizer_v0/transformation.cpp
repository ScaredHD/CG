#include "transformation.h"

Eigen::Matrix4d matrix::translation(const double& x, const double y, const double z) {
	Eigen::Matrix4d mat;
	mat <<
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1;
	return mat;
}

Eigen::Matrix4d matrix::translation(Eigen::Vector3d v) {
	auto x = v.x();
	auto y = v.y();
	auto z = v.z();
	return translation(x, y, z);
}

Eigen::Matrix4d matrix::rotationX(const double& deg) {
	auto t = deg / 180 * pi;
	Eigen::Matrix4d mat;
	mat <<
		1, 0, 0, 0,
		0, cos(t), -sin(t), 0,
		0, sin(t), cos(t), 0,
		0, 0, 0, 1;
	return mat;
}

Eigen::Matrix4d matrix::rotationY(const double& deg) {
	auto t = deg / 180 * pi;
	Eigen::Matrix4d mat;
	mat <<
		cos(t), 0, sin(t), 0,
		0, 1, 0, 0,
		-sin(t), 0, cos(t), 0,
		0, 0, 0, 1;
	return mat;
}

Eigen::Matrix4d matrix::rotationZ(const double& deg) {
	auto t = deg / 180 * pi;
	Eigen::Matrix4d mat;
	mat <<
		cos(t), -sin(t), 0, 0,
		sin(t), cos(t), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1;
	return mat;
}

Eigen::Matrix4d matrix::rotation(const Eigen::Vector3d& n, double deg) {
	// TODO: fix this function
	auto a = deg / 180 * pi;
	auto I = Eigen::Matrix3d::Identity();
	Eigen::Matrix3d m1 = n * n.transpose();
	Eigen::Matrix3d m2;
	m2 <<
		0, -n.z(), n.y(),
		n.z(), 0, -n.x(),
		-n.y(), n.x(), 0;
	Eigen::MatrixXd mat = cos(a) * I + (1 - cos(a)) * m1 + sin(a) * m2;
	mat.conservativeResize(4, 4);
	mat(0, 3) = mat(1, 3) = mat(2, 3) = 0;
	mat(3, 0) = mat(3, 1) = mat(3, 2) = 0;
	mat(3, 3) = 1;
	return mat;
}

Eigen::Vector3d matrix::rotation(const Eigen::Vector3d& v, const Eigen::Vector3d axis, double deg) {
	auto k = axis.normalized();
	auto a = deg / 180 * pi;
	Eigen::Vector3d ret = cos(a) * v + cross(k, v) * sin(a) + k * (k.dot(v)) * (1 - cos(a));
	return ret;
}
