#include "geometry.h"

void Triangle::setVertices(Eigen::Vector3d v0_, Eigen::Vector3d v1_, Eigen::Vector3d v2_) {
	v0 = v0_;
	v1 = v1_;
	v2 = v2_;
}

void Triangle::setColor(cv::Vec3b color) {
	color0 = color1 = color2 = color;
}

void Triangle::setColor(cv::Vec3b c0, cv::Vec3b c1, cv::Vec3b c2) {
	color0 = c0;
	color1 = c1;
	color2 = c2;
}

cv::Vec3b Triangle::randomColor() {
	std::uniform_int_distribution<int> c(0, 255);
	return cv::Vec3b(c(gen), c(gen), c(gen));
}

void Triangle::print() const {
	std::cout << "v0 = (" << v0.x() << ", " << v0.y() << ", " << v0.z() << ")\n";
	std::cout << "v1 = (" << v1.x() << ", " << v1.y() << ", " << v1.z() << ")\n";
	std::cout << "v2 = (" << v2.x() << ", " << v2.y() << ", " << v2.z() << ")\n\n";
}

void Line::setVertices(double ux, double uy, double uz, double vx, double vy, double vz) {
	u = Eigen::Vector3d(ux, uy, uz);
	v = Eigen::Vector3d(vx, vy, vz);
}

void Line::setVertices(Eigen::Vector3d u_, Eigen::Vector3d v_) {
	u = u_;
	v = v_;
}

void Line::setColor(cv::Vec3b color) {
	ucolor = vcolor = color;
}
