#pragma once
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
    Shader() = default;
    Shader(const std::string& vertexShaderPath, const std::string& fragShaderPath) {
        std::ifstream vShaderFile(vertexShaderPath);
        std::ifstream fShaderFile(fragShaderPath);
        if (!vShaderFile.is_open() || !fShaderFile.is_open()) {
            std::cout << "failed to open shader file\n";
        }

        std::stringstream vShaderStream;
        std::stringstream fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        auto        vShaderString = vShaderStream.str();
        auto        fShaderString = fShaderStream.str();
        const char* vShaderCode   = vShaderString.c_str();
        const char* fShaderCode   = fShaderString.c_str();

        GLuint vertexShader   = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
        glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);

        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
        };
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
        };

        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    GLuint id;

    void use() const { glUseProgram(id); }

private:
};