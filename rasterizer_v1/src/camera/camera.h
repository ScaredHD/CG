#pragma once

#include <string>

#include "vec.h"
#include "matrix.h"

// coordinate system: right-handed, y-up

struct Camera {
    Camera(Vec3 location, Vec3 lookAt, Vec3 up) : location(location), lookAt(lookAt), up(up) {}

    double pitch() const;
    double yaw() const;

    Mat4 viewTransformation() const;
    Mat4 projectionTransformation() const;

    double top() const;
    double bottom() const;
    double left() const;
    double right() const;

    Vec3 location;
    Vec3 lookAt;  // direction of looking
    Vec3 up;

    double hFovDeg = 90.0;
    double aspectRatio = 16.0 / 9.0;
    double zNear = -0.1;
    double zFar = -100.0;

    std::string type = "base";

  private:
};

struct FpsCamera : public Camera {
    FpsCamera(Vec3 location, Vec3 lookAt, Vec3 up = {{0.0, 1.0, 0.0}})
        : Camera(location, lookAt, up) {
        type = "fps";
    }

    void moveForward(double dist);
    void moveRight(double dist);
    void lookUp(double deg);
    void lookRight(double deg);

    double moveSpeed = 0.001;
    double mouseSensitivity = 0.04;
};

struct OrbitCamera : public Camera {};