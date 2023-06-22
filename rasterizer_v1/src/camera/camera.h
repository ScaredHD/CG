#pragma once

#include <string>

#include "vec.h"
#include "matrix.h"

// coordinate system: right-handed, y-up

enum class CameraType { base, fps, orbit };

class Camera {
  public:
    Camera(Vec3 location, Vec3 lookAt, Vec3 up) : location(location), lookAt(lookAt), up(up) {}

    double pitch() const;
    double yaw() const;

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

    CameraType type = CameraType::base;

  private:
};

struct FpsCamera : public Camera {
    FpsCamera(Vec3 location, Vec3 lookAt, Vec3 up = {{0.0, 1.0, 0.0}})
        : Camera(location, lookAt, up) {
        type = CameraType::fps;
    }

    void moveForward(double dist);
    void moveRight(double dist);
    void lookUp(double deg);
    void lookRight(double deg);

    double moveSpeed = 0.01;
    double mouseSensitivity = 0.04;
};

struct OrbitCamera : public Camera {};

void printCameraPose(const Camera& cam);