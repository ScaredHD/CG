#include "math.h"

double clamp(double x, double low, double high) {
	return std::max(low, std::min(x, high));
}

Eigen::Vector3d cross(const Eigen::Vector3d& u, const Eigen::Vector3d& v) {
	auto x1 = u.x();
	auto y1 = u.y();
	auto z1 = u.z();
	auto x2 = v.x();
	auto y2 = v.y();
	auto z2 = v.z();
	auto x = y1 * z2 - y2 * z1;
	auto y = -(x1 * z2 - x2 * z1);
	auto z = x1 * y2 - x2 * y1;
	return Eigen::Vector3d(x, y, z);
}

Eigen::Vector3d vecminus(const Eigen::Vector3d& u, const Eigen::Vector3d& v) {
	auto x = u.x() - v.x();
	auto y = u.y() - v.y();
	auto z = u.z() - v.z();
	return Eigen::Vector3d(x, y, z);
}
