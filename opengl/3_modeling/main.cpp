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
#include <random>
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

int main() {
    auto* window = init();

    Shader shader("../shader/object.vs", "../shader/object.fs");
    Shader outlineShader("../shader/object.vs", "../shader/outline.fs");

    Model model("../model/wingman/wingman.obj");
    model.pos = vec3(0.0f);

    // render loop
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);

        /* background */
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glStencilMask(0xff);  // enable writing of stencil buffer, otherwise
                              // clear may not take effect
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw object
        shader.use();

        model.scale = vec3(0.1f);
        auto modelMatrix = model.modelMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.id, "model"), 1,
                           GL_FALSE, value_ptr(modelMatrix));
        cam.sendToShader(shader, "view", "projection");
        model.draw(shader);

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
}

GLFWwindow* init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Modeling",
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