#pragma once
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"

#include <string>

class Camera {
public:
    Camera() = default;
    Camera(glm::vec3 pos, glm::vec3 gaze = glm::vec3(0.0F, 0.0F, -1.0F), float pitch = 0.0F,
           float yaw = -90.0F)
        : pos(pos),
          gaze(gaze),
          pitch(pitch),
          yaw(yaw) {}
    Camera(float posX, float posY, float posZ, float gazeX = 0.0F, float gazeY = 0.0F,
           float gazeZ = 1.0F, float pitch = 0.0F, float yaw = -90.F)
        : pos(glm::vec3(posX, posY, posZ)),
          gaze(glm::vec3(gazeX, gazeY, gazeZ)),
          pitch(pitch),
          yaw(yaw) {}

    double    speed = 2.5F;
    glm::vec3 pos = glm::vec3(0.0F, 0.0F, 0.0F);
    glm::vec3 gaze = glm::vec3(0.0F, 0.0F, -1.0F);

    float fov = 45.0F;
    float aspectRatio = 800.0F / 600.F;
    float zNear = 0.1F;
    float zFar = 100.F;

    float sensitivity = 0.05F;
    float radius = 1.0F;
    float pitch = 0.0F;
    float yaw = -90.0F;

    float fovMax = 110.0F;
    float fovMin = 1.0F;
    float pitchMax = 89.0F;
    float pitchMin = -89.0F;

    bool restrictToXZPlane = false;

    // matrices
    glm::mat4 viewMatrix() const { return glm::lookAt(pos, pos + gaze, up); }
    glm::mat4 projectionMatrix() const {
        return glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
    }

    // movement
    void moveForward(double delta) {
        auto c = static_cast<float>(speed * delta);
        auto direction = restrictToXZPlane ? glm::normalize(glm::vec3(gaze.x, 0.0F, gaze.z)) : gaze;
        pos += c * direction;
    }
    void moveBackward(double delta) {
        auto c = static_cast<float>(speed * delta);
        auto direction = restrictToXZPlane ? glm::normalize(glm::vec3(gaze.x, 0.0F, gaze.z)) : gaze;
        pos -= c * direction;
    }
    void moveLeft(double delta) {
        pos -= glm::normalize(glm::cross(gaze, up)) * static_cast<float>(speed * delta);
    }
    void moveRight(double delta) {
        pos += glm::normalize(glm::cross(gaze, up)) * static_cast<float>(speed * delta);

    }

    // process input
    void processCursor(double dx, double dy) {
        yaw += static_cast<float>(sensitivity * dx);
        pitch += static_cast<float>(sensitivity * dy);
        pitch = glm::clamp(pitch, pitchMin, pitchMax);

        glm::vec3 direction;
        auto      p = glm::radians(pitch);
        auto      y = glm::radians(yaw);
        direction.x = radius * std::cos(p) * std::cos(y);
        direction.y = radius * std::sin(p);
        direction.z = radius * std::cos(p) * std::sin(y);
        gaze = glm::normalize(direction);
    }

    void processScroll(double dy) {
        fov -= static_cast<float>(dy);
        fov = glm::clamp(fov, fovMin, fovMax);
    }

    std::string camPosString() const {
        std::string ret("(");
        ret += std::to_string(pos.x);
        ret += ", ";
        ret += std::to_string(pos.y);
        ret += ", ";
        ret += std::to_string(pos.z);
        ret += ")";
        return ret;
    }

private:
    glm::vec3 up = glm::vec3(0.0F, 1.0F, 0.0F);
};