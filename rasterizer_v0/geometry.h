#pragma once

#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>
#include <random>
#include <iostream>

static std::random_device rd;
static std::mt19937 gen(rd());

struct Triangle {
	Eigen::Vector3d v0;
	Eigen::Vector3d v1;
	Eigen::Vector3d v2;
	cv::Vec3b color0;
	cv::Vec3b color1;
	cv::Vec3b color2;

	Triangle(Eigen::Vector3d v0_ = { 0, 0, 0 },
			 Eigen::Vector3d v1_ = { 0, 0, 0 },
			 Eigen::Vector3d v2_ = { 0, 0, 0 },
			 cv::Vec3b c0 = { 0, 0, 0 },
			 cv::Vec3b c1 = { 0, 0, 0 },
			 cv::Vec3b c2 = { 0, 0, 0 })
		: v0(v0_), v1(v1_), v2(v2_) {
		color0 = c0;
		color1 = c1;
		color2 = c2;
	}

	void setVertices(Eigen::Vector3d v0_, Eigen::Vector3d v1_, Eigen::Vector3d v2_);
	void setColor(cv::Vec3b color);
	void setColor(cv::Vec3b c0, cv::Vec3b c1, cv::Vec3b c2);
	static cv::Vec3b randomColor();
	void print() const;
};

struct Line {
	Eigen::Vector3d u;
	Eigen::Vector3d v;
	cv::Vec3b ucolor;
	cv::Vec3b vcolor;

	Line(double ux, double uy, double uz,
		 double vx, double vy, double vz,
		 cv::Vec3b ucolor_ = { 0, 0, 0 }, cv::Vec3b vcolor_ = { 0, 0, 0 })
		: u(Eigen::Vector3d(ux, uy, uz)), v(Eigen::Vector3d(vx, vy, vz)),
		ucolor(ucolor_), vcolor(vcolor_) {}

	Line(Eigen::Vector3d u_, Eigen::Vector3d v_, cv::Vec3b ucolor_ = { 0, 0, 0 }, cv::Vec3b vcolor_ = { 0, 0, 0 })
		: u(u_), v(v_), ucolor(ucolor_), vcolor(vcolor_) {}

	void setVertices(double ux, double uy, double uz,
					 double vx, double vy, double vz);
	void setVertices(Eigen::Vector3d u_, Eigen::Vector3d v_);
	void setColor(cv::Vec3b color);
};
