#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

class Light {
  public:
    Light() = default;
    Light(float x, float y, float z, glm::vec3 color)
        : pos(x, y, z), color(color) {}

    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec3 color = glm::vec3(1.0f);

    void sendToShader(const Shader& shader, const char* posVarname,
                      const char* colorVarname) {
        glUniform3fv(glGetUniformLocation(shader.id, posVarname), 1,
                     glm::value_ptr(pos));
        glUniform3fv(glGetUniformLocation(shader.id, colorVarname), 1,
                     glm::value_ptr(color));
    }

  private:
};