// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <stb_image.h>

#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "camera.h"
#include "light.h"
#include "model.h"
#include "shader.h"

using namespace glm;

GLFWwindow* init();
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void cursorPosCallBack(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double dx, double dy);

int windowWidth = 800;
int windowHeight = 600;

double deltaTime = 0.0f;
double lastFrame = 0.0f;

bool cursorFirstEntering = true;
auto cursorX = static_cast<float>(windowWidth) / 2;
auto cursorY = static_cast<float>(windowHeight) / 2;

Camera cam(vec3(0.0f, 0.0f, 3.0f));
Light light;

// clang-format off
std::vector<float> cubeVertices = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};
// clang-format on

bool antialiasing = false;

int main() {
    auto* window = init();

    Shader shader("../shader/object.vs", "../shader/object.fs");

    GLuint cubeVbo;
    glGenBuffers(1, &cubeVbo);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(float),
                 cubeVertices.data(), GL_DYNAMIC_DRAW);

    GLuint cubeVao;
    glGenVertexArrays(1, &cubeVao);
    glBindVertexArray(cubeVao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);

        if (antialiasing)
            glEnable(GL_MULTISAMPLE);
        else
            glDisable(GL_MULTISAMPLE);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setMat4("model", mat4(1.0f));
        shader.setMat4("view", cam.viewMatrix());
        shader.setMat4("projection", cam.projectionMatrix());

        // draw scene
        glBindVertexArray(cubeVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void cursorPosCallBack(GLFWwindow* window, double xPos, double yPos) {
    // prevent sudden jump when entering window
    if (cursorFirstEntering) {
        cursorX = static_cast<float>(xPos);
        cursorY = static_cast<float>(yPos);
        cursorFirstEntering = false;
    }
    auto dx = xPos - cursorX;
    auto dy = cursorY - yPos;
    cursorX = static_cast<float>(xPos);
    cursorY = static_cast<float>(yPos);
    cam.processCursor(dx, dy);
}

void scrollCallback(GLFWwindow* window, double dx, double dy) {
    cam.processScroll(dy);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.moveForward(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam.moveBackward(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam.moveLeft(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam.moveRight(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        antialiasing = true;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        antialiasing = false;
    }
}

GLFWwindow* init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);  // MSAA

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Antialiasing",
                                          nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, cursorPosCallBack);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, scrollCallback);

    stbi_set_flip_vertically_on_load(false);

    return window;
}