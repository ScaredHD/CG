#pragma once
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/type_ptr.hpp"
// clang-format on

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
  public:
    Shader() = default;
    Shader(const std::string& vertexShaderPath,
           const std::string& fragShaderPath) {
        std::ifstream vShaderFile(vertexShaderPath);
        std::ifstream fShaderFile(fragShaderPath);
        if (!vShaderFile.is_open() || !fShaderFile.is_open()) {
            std::cout << "failed to open shader file\n";
        }

        std::stringstream vShaderStream;
        std::stringstream fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        auto vShaderString = vShaderStream.str();
        auto fShaderString = fShaderStream.str();
        const char* vShaderCode = vShaderString.c_str();
        const char* fShaderCode = fShaderString.c_str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
        glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);

        int success;
        std::array<char, 512> infoLog;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog.data());
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog.data() << "\n";
        };
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog.data());
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                      << infoLog.data() << "\n";
        };

        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 512, nullptr, infoLog.data());
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog.data() << "\n";
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    GLuint id;

    void use() const { glUseProgram(id); }

    void setInt(const std::string& varname, int value) const {
        glUniform1i(glGetUniformLocation(id, varname.c_str()), value);
    }

    void setVec3(const std::string& varname, const glm::vec3& v) const {
        glUniform3f(glGetUniformLocation(id, varname.c_str()), v.x, v.y, v.z);
    }

    void setMat4(const std::string& varname, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(id, varname.c_str()), 1,
                           GL_FALSE, glm::value_ptr(mat));
    }

  private:
};