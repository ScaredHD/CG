#pragma once

#include <Eigen/Core>
#include "math.h"
#include "transformation.h"

struct Camera {
	Eigen::Vector3d pos;
	Eigen::Vector3d gaze;
	Eigen::Vector3d top;
	double fovDeg;
	double aspectRatio;
	double zNear;
	double zFar;

	void setGaze(Eigen::Vector3d g) { gaze = g.normalized(); }
	void setTop(Eigen::Vector3d t) { top = t.normalized(); }

	Camera(Eigen::Vector3d pos_ = Eigen::Vector3d(0.0, 0.0, 0.0),
		   Eigen::Vector3d gaze_ = Eigen::Vector3d(0.0, 0.0, -1.0),
		   Eigen::Vector3d top_ = Eigen::Vector3d(0.0, 1.0, 0.0),
		   double fovDeg_ = 90,
		   double aspectRatio_ = 1.0,
		   double zNear_ = 1.0,
		   double zFar_ = 100.0) {
		pos = pos_;
		gaze = gaze_.normalized();
		top = top_.normalized();
		fovDeg = fovDeg_;
		aspectRatio = aspectRatio_;
		zNear = zNear_;
		zFar = zFar_;
	}

	// camera movement
	// dolly/forward: camera moving forward/backward along gazing direction
	void forward(double dist) { pos += dist * gaze.normalized(); }
	void dolly(double dist) { forward(dist); }
	// truck/right: camera moving left/right along cross(g, t)
	void right(double dist) { pos += dist * cross(gaze.normalized(), top.normalized()); }
	void truck(double dist) { right(dist); }
	// pedestal/up: camera moving up/down along top direction
	void up(double dist) { pos += dist * top.normalized(); }
	void pedestal(double dist) { up(dist); }
	// tilt/lookup: camera looking up (position fixed)
	void lookup(double deg);
	void tilt(double deg) { lookup(deg); }
	// pan/lookright: camera looking right (position fixed)
	void lookleft(double deg);
	void pan(double deg) { lookleft(deg); }
};