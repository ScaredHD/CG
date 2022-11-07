// clang-format off
#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/matrix.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// clang-format on

#include <cmath>
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
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

Camera cam(vec3(0.0f, 0.0f, 3.0f));
Light light(vec3(0.0, 5.0f, 0.0f), vec3(1.0f));

int windowWidth = 800;
int windowHeight = 600;

double deltaTime = 0.0f;
double lastFrame = 0.0f;

bool cursorFirstEntering = true;
float cursorX = float(windowWidth) / 2;
float cursorY = float(windowHeight) / 2;

bool rotationMode = false;
bool onClick = false;
double clickStartX;
double clickStartY;
mat4 rotationMatrix(1.0f);
mat4 lastModel(1.0f);

mat4 viewport(1.0f);
mat4 toNdc(1.0f);

// clang-format off
std::vector<float> lightCube = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
// clang-format on

int main() {
    auto* window = init();

    // shaders
    Shader shader("../shader/object.vs", "../shader/object.fs");
    Shader lightShader("../shader/light.vs", "../shader/light.fs");

    Model teapot("../model/teapot/teapot.obj");

    GLuint lightCubeVbo;
    glGenBuffers(1, &lightCubeVbo);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVbo);
    glBufferData(GL_ARRAY_BUFFER, lightCube.size() * sizeof(float),
                 lightCube.data(), GL_DYNAMIC_DRAW);

    GLuint lightCubeVao;
    glGenVertexArrays(1, &lightCubeVao);
    glBindVertexArray(lightCubeVao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glBindVertexArray(0);

    // render loop
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // background
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // preview rotation
        auto model = rotationMatrix * lastModel;
        model = scale(model, vec3(0.2f));
        shader.setMat4("model", model);
        shader.setMat4("view", cam.viewMatrix());
        shader.setMat4("projection", cam.projectionMatrix());
        shader.setVec3("lightInWorld", light.pos);
        shader.setVec3("lightColor", light.color);
        shader.setVec3("objectColor", vec3(0.5f, 0.3f, 0.1f));

        // drawing
        shader.use();
        teapot.draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();

        auto currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
    }

    glfwTerminate();
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
}

void cursorPosCallBack(GLFWwindow* window, double xPos, double yPos) {
    // prevent sudden jump when entering window
    if (cursorFirstEntering) {
        cursorX = static_cast<float>(xPos);
        cursorY = static_cast<float>(yPos);
        cursorFirstEntering = false;
    }

    if (rotationMode) {
        if (onClick) {
            // reverse y-axis
            glm::vec4 cursorOnScreen(xPos, windowHeight - yPos, 0.0f, 1.0f);
            glm::vec4 startOnScreen(clickStartX, windowHeight - clickStartY,
                                    0.0f, 1.0f);

            // map to NDC (inverse of viewport trans)
            auto cursor = toNdc * cursorOnScreen;
            cursor.x = glm::clamp(cursor.x, -1.0f, 1.0f);
            cursor.y = glm::clamp(cursor.y, -1.0f, 1.0f);
            float sumOfSquare = cursor.x * cursor.x + cursor.y * cursor.y;
            cursor.z = sumOfSquare < 1 ? std::sqrt(1 - sumOfSquare) : 0;

            auto start = toNdc * startOnScreen;
            start.x = glm::clamp(start.x, -1.0f, 1.0f);
            start.y = glm::clamp(start.y, -1.0f, 1.0f);
            sumOfSquare = start.x * start.x + start.y * start.y;
            start.z = sumOfSquare < 1 ? std::sqrt(1 - sumOfSquare) : 0;

            // calculate axis
            auto v1 = glm::vec3(start) / start.w;
            auto v2 = glm::vec3(cursor) / cursor.w;
            auto rotAxis = glm::normalize(glm::cross(v1, v2));

            // calculate angle
            auto rotAngle = std::acos(glm::dot(v1, v2) /
                                      (glm::length(v1) * glm::length(v2)));

            // calculate rotation matrix
            rotationMatrix = glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis);
        }
    } else {
        auto dx = xPos - cursorX;
        auto dy = cursorY - yPos;
        cam.processCursor(dx, dy);
        cursorX = static_cast<float>(xPos);
        cursorY = static_cast<float>(yPos);
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS && !onClick) {
            onClick = true;
            glfwGetCursorPos(window, &clickStartX, &clickStartY);
        }
        if (action == GLFW_RELEASE && onClick) {
            onClick = false;
            // fix current rotation state
            lastModel = rotationMatrix * lastModel;
            // reset rotation matrix, otherwise it will be applied agian on
            // already fixed model in next render loop preview
            rotationMatrix = glm::mat4(1.0f);
        }
    }
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

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        lastModel = glm::mat4(1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        rotationMode = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        rotationMode = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        cursorFirstEntering = true;
    }
}

void scrollCallback(GLFWwindow* window, double dx, double dy) {
    cam.processScroll(dy);
}

GLFWwindow* init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =
        glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
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
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    viewport = glm::translate(
        viewport,
        glm::vec3(float(windowWidth) / 2.0f, float(windowHeight) / 2.0f, 0.0f));
    viewport = glm::scale(
        viewport,
        glm::vec3(float(windowWidth) / 2.0f, float(windowHeight) / 2.0f, 1.0f));
    toNdc = glm::inverse(viewport);

    return window;
}
