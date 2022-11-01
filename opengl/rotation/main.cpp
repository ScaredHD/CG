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

#define STB_IMAGE_IMPLEMENTATION
#include "include/camera.h"
#include "include/shader.h"
#include "include/stb_image.h"

#include <cmath>
#include <iostream>
#include <random>
#include <vector>

GLFWwindow* init();
void        frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void        processInput(GLFWwindow* window);
void        cursorPosCallBack(GLFWwindow* window, double xPos, double yPos);
void        scrollCallback(GLFWwindow* window, double dx, double dy);
void        mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

// clang-format off
std::vector<float> vertices = {
    -0.5F, -0.5F, -0.5F,  0.0F, 0.0F,
     0.5F, -0.5F, -0.5F,  1.0F, 0.0F,
     0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
     0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
    -0.5F,  0.5F, -0.5F,  0.0F, 1.0F,
    -0.5F, -0.5F, -0.5F,  0.0F, 0.0F,

    -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
     0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
     0.5F,  0.5F,  0.5F,  1.0F, 1.0F,
     0.5F,  0.5F,  0.5F,  1.0F, 1.0F,
    -0.5F,  0.5F,  0.5F,  0.0F, 1.0F,
    -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,

    -0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
    -0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
    -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
    -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
    -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
    -0.5F,  0.5F,  0.5F,  1.0F, 0.0F,

     0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
     0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
     0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
     0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
     0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
     0.5F,  0.5F,  0.5F,  1.0F, 0.0F,

    -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
     0.5F, -0.5F, -0.5F,  1.0F, 1.0F,
     0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
     0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
    -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
    -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,

    -0.5F,  0.5F, -0.5F,  0.0F, 1.0F,
     0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
     0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
     0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
    -0.5F,  0.5F,  0.5F,  0.0F, 0.0F,
    -0.5F,  0.5F, -0.5F,  0.0F, 1.0F
};
// clang-format on

Camera cam(glm::vec3(0.0F, 0.0F, 3.0F));

int windowWidth = 800;
int windowHeight = 600;

double deltaTime = 0.0F;
double lastFrame = 0.0F;

bool  cursorFirstEntering = true;
float cursorX = float(windowWidth) / 2;
float cursorY = float(windowHeight) / 2;

bool      rotationMode = false;
bool      onClick = false;
double    clickStartX;
double    clickStartY;
glm::mat4 rotationMatrix(1.0F);
glm::mat4 lastModel(1.0F);

glm::mat4 viewport(1.0F);
glm::mat4 toNdc(1.0F);

int main() {
    auto* window = init();

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(vertices.size() * sizeof(float)), vertices.data(),
                 GL_DYNAMIC_DRAW);

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // attr0 pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // attr2 tex
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // shaders
    Shader shader("../shader/vertex_shader.txt", "../shader/fragment_shader.txt");
    shader.use();

    // image
    stbi_set_flip_vertically_on_load(true);
    int            width0;
    int            width1;
    int            channelCnt0;
    int            channelCnt1;
    int            height0;
    int            height1;
    unsigned char* img0 = stbi_load("../img/container.jpg", &width0, &height0, &channelCnt0, 0);
    printf("container (width, height, #channel): (%d, %d, %d)\n", width0, height0, channelCnt0);
    unsigned char* img1 = stbi_load("../img/awesomeface.png", &width1, &height1, &channelCnt1, 0);
    printf("awesomeface (width, height, #channel): (%d, %d, %d)\n", width1, height1, channelCnt1);

    // textures
    GLuint tex0;
    GLuint tex1;
    glGenTextures(1, &tex0);
    glGenTextures(1, &tex1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex0);  // target-object binding
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width0, height0, 0, GL_RGB, GL_UNSIGNED_BYTE, img0);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex1);  // target-object binding
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, img1);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // sampler-target binding
    glUniform1i(glGetUniformLocation(shader.id, "tex0"), 0);
    glUniform1i(glGetUniformLocation(shader.id, "tex1"), 1);

    glEnable(GL_DEPTH_TEST);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // background
        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef DEBUG_CAM
        printf("pos: (%.1f, %.1f, %.1f)\t(pitch,yaw,fov): (%.2f, %.2f, %.2f)\n", cam.pos.x,
               cam.pos.y, cam.pos.z, cam.pitch, cam.yaw, cam.fov);
#endif

        // preview rotation
        auto model = rotationMatrix * lastModel;
        glUniformMatrix4fv(glGetUniformLocation(shader.id, "model"), 1, GL_FALSE,
                           glm::value_ptr(model));

        glm::mat4 view = cam.viewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.id, "view"), 1, GL_FALSE,
                           glm::value_ptr(view));

        glm::mat4 projection = cam.projectionMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.id, "projection"), 1, GL_FALSE,
                           glm::value_ptr(projection));

        // drawing
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

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
            glm::vec4 cursorOnScreen(xPos, windowHeight - yPos, 0.0F, 1.0F);
            glm::vec4 startOnScreen(clickStartX, windowHeight - clickStartY, 0.0F, 1.0F);

            // map to NDC (inverse of viewport trans)
            auto cursor = toNdc * cursorOnScreen;
            cursor.x = glm::clamp(cursor.x, -1.0F, 1.0F);
            cursor.y = glm::clamp(cursor.y, -1.0F, 1.0F);
            float sumOfSquare = cursor.x * cursor.x + cursor.y * cursor.y;
            cursor.z = sumOfSquare < 1 ? std::sqrt(1 - sumOfSquare) : 0;

            auto start = toNdc * startOnScreen;
            start.x = glm::clamp(start.x, -1.0F, 1.0F);
            start.y = glm::clamp(start.y, -1.0F, 1.0F);
            sumOfSquare = start.x * start.x + start.y * start.y;
            start.z = sumOfSquare < 1 ? std::sqrt(1 - sumOfSquare) : 0;

            // calculate axis
            auto v1 = glm::vec3(start) / start.w;
            auto v2 = glm::vec3(cursor) / cursor.w;
            auto rotAxis = glm::normalize(glm::cross(v1, v2));

            // calculate angle
            auto rotAngle = std::acos(glm::dot(v1, v2) / (glm::length(v1) * glm::length(v2)));

            // calculate rotation matrix
            rotationMatrix = glm::rotate(glm::mat4(1.0F), rotAngle, rotAxis);
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
            // reset rotation matrix, otherwise it will be applied agian on already fixed model in
            // next render loop preview
            rotationMatrix = glm::mat4(1.0F);
        }
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { cam.moveForward(deltaTime); }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { cam.moveBackward(deltaTime); }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { cam.moveLeft(deltaTime); }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { cam.moveRight(deltaTime); }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { lastModel = glm::mat4(1.0F); }
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
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
        viewport, glm::vec3(float(windowWidth) / 2.0F, float(windowHeight) / 2.0F, 0.0F));
    viewport = glm::scale(viewport,
                          glm::vec3(float(windowWidth) / 2.0F, float(windowHeight) / 2.0F, 1.0F));
    toNdc = glm::inverse(viewport);

    return window;
}
