#pragma once
#include <string>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader.h"

class Camera {
  public:
    Camera() = default;
    Camera(glm::vec3 pos, glm::vec3 gaze = glm::vec3(0.0f, 0.0f, -1.0f),
           float pitch = 0.0f, float yaw = -90.0f)
        : pos(pos), gaze(gaze), pitch(pitch), yaw(yaw) {}
    Camera(float posX, float posY, float posZ, float gazeX = 0.0f,
           float gazeY = 0.0f, float gazeZ = 1.0f, float pitch = 0.0f,
           float yaw = -90.F)
        : pos(glm::vec3(posX, posY, posZ)),
          gaze(glm::vec3(gazeX, gazeY, gazeZ)),
          pitch(pitch),
          yaw(yaw) {}

    double speed = 2.5f;
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 gaze = glm::vec3(0.0f, 0.0f, -1.0f);

    float fov = 45.0f;
    float aspectRatio = 800.0f / 600.F;
    float zNear = 0.1f;
    float zFar = 100.F;

    float sensitivity = 0.05f;
    float radius = 1.0f;
    float pitch = 0.0f;
    float yaw = -90.0f;

    float fovMax = 110.0f;
    float fovMin = 1.0f;
    float pitchMax = 89.0f;
    float pitchMin = -89.0f;

    bool restrictToXZPlane = false;

    // matrices
    glm::mat4 viewMatrix() const { return glm::lookAt(pos, pos + gaze, up); }
    glm::mat4 projectionMatrix() const {
        return glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
    }

    // movement
    void moveForward(double delta) {
        auto c = static_cast<float>(speed * delta);
        auto direction = restrictToXZPlane
                             ? glm::normalize(glm::vec3(gaze.x, 0.0f, gaze.z))
                             : gaze;
        pos += c * direction;
    }
    void moveBackward(double delta) {
        auto c = static_cast<float>(speed * delta);
        auto direction = restrictToXZPlane
                             ? glm::normalize(glm::vec3(gaze.x, 0.0f, gaze.z))
                             : gaze;
        pos -= c * direction;
    }
    void moveLeft(double delta) {
        pos -= glm::normalize(glm::cross(gaze, up)) *
               static_cast<float>(speed * delta);
    }
    void moveRight(double delta) {
        pos += glm::normalize(glm::cross(gaze, up)) *
               static_cast<float>(speed * delta);
    }

    // process input
    void processCursor(double dx, double dy) {
        yaw += static_cast<float>(sensitivity * dx);
        pitch += static_cast<float>(sensitivity * dy);
        pitch = glm::clamp(pitch, pitchMin, pitchMax);

        glm::vec3 direction;
        auto p = glm::radians(pitch);
        auto y = glm::radians(yaw);
        direction.x = radius * std::cos(p) * std::cos(y);
        direction.y = radius * std::sin(p);
        direction.z = radius * std::cos(p) * std::sin(y);
        gaze = glm::normalize(direction);
    }

    void processScroll(double dy) {
        fov -= static_cast<float>(dy);
        fov = glm::clamp(fov, fovMin, fovMax);
    }

    // interaction with shader
    void sendToShader(const Shader& shader, const char* viewMatVarname,
                      const char* projectionMatVarname) const {
        auto viewMat = viewMatrix();
        auto projectionMat = projectionMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.id, viewMatVarname), 1,
                           GL_FALSE, glm::value_ptr(viewMat));
        glUniformMatrix4fv(
            glGetUniformLocation(shader.id, projectionMatVarname), 1, GL_FALSE,
            glm::value_ptr(projectionMat));
    }

  private:
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
};