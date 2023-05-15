#pragma once

#include "vec.h"

struct Camera {
    Vec3 pos;
    Vec3 gaze;
    Vec3 top;
    double fovDeg;
    double aspectRatio;
    double zNear;
    double zFar;

    Camera(Vec3 pos = Vec3(0.0, 0.0, 0.0), Vec3 gaze = Vec3(0.0, 0.0, -1.0),
           Vec3 top = Vec3(0.0, 1.0, 0.0), double fovDeg = 90, double aspectRatio = 1.0,
           double zNear = 1.0, double zFar = 100.0)
        : pos(pos),
          gaze(gaze.normalized()),
          top(top.normalized()),
          fovDeg(fovDeg),
          aspectRatio(aspectRatio),
          zNear(zNear),
          zFar(zFar) {}

    // camera movement
    void moveForward(double dist);
    void moveRight(double dist);
    void moveUp(double dist);
    void lookup(double deg);
    void lookleft(double deg);
};