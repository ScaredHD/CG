#pragma once

#include <string>

#include "vec.h"

// coordinate system: right-handed, y-up

struct Camera {
    Camera(Vec3 location, Vec3 lookAt, Vec3 up)
        : location(location), lookAt(lookAt), up(up) {}

    double pitch() const;
    double yaw() const;

    Vec3 location;
    Vec3 lookAt;  // direction of looking
    Vec3 up;

    double fovDeg = 90.0;
    double aspectRatio = 16.0 / 9.0;
    double zNear = 1.0;
    double zFar = 100.0;

    std::string type = "base";

  private:
};

struct FpsCamera : public Camera {
    FpsCamera(Vec3 location, Vec3 lookAt, Vec3 up = {0.0, 1.0, 0.0})
        : Camera(location, lookAt, up) {
        type = "fps";
    }

    void moveForward(double dist);
    void moveRight(double dist);
    void lookUp(double deg);
    void lookRight(double deg);

    double moveSpeed = 1.0;
    double mouseSensitivity = 0.5;
};

struct OrbitCamera : public Camera {};