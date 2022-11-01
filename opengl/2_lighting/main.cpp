// clang-format off
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

// clang-format off
std::vector<float> vertices = {
    // positions          // normals           // texture coords
    -0.5F, -0.5F, -0.5F,  0.0F,  0.0F, -1.0F,  0.0F, 0.0F,
     0.5F, -0.5F, -0.5F,  0.0F,  0.0F, -1.0F,  1.0F, 0.0F,
     0.5F,  0.5F, -0.5F,  0.0F,  0.0F, -1.0F,  1.0F, 1.0F,
     0.5F,  0.5F, -0.5F,  0.0F,  0.0F, -1.0F,  1.0F, 1.0F,
    -0.5F,  0.5F, -0.5F,  0.0F,  0.0F, -1.0F,  0.0F, 1.0F,
    -0.5F, -0.5F, -0.5F,  0.0F,  0.0F, -1.0F,  0.0F, 0.0F,

    -0.5F, -0.5F,  0.5F,  0.0F,  0.0F, 1.0F,   0.0F, 0.0F,
     0.5F, -0.5F,  0.5F,  0.0F,  0.0F, 1.0F,   1.0F, 0.0F,
     0.5F,  0.5F,  0.5F,  0.0F,  0.0F, 1.0F,   1.0F, 1.0F,
     0.5F,  0.5F,  0.5F,  0.0F,  0.0F, 1.0F,   1.0F, 1.0F,
    -0.5F,  0.5F,  0.5F,  0.0F,  0.0F, 1.0F,   0.0F, 1.0F,
    -0.5F, -0.5F,  0.5F,  0.0F,  0.0F, 1.0F,   0.0F, 0.0F,

    -0.5F,  0.5F,  0.5F, -1.0F,  0.0F,  0.0F,  1.0F, 0.0F,
    -0.5F,  0.5F, -0.5F, -1.0F,  0.0F,  0.0F,  1.0F, 1.0F,
    -0.5F, -0.5F, -0.5F, -1.0F,  0.0F,  0.0F,  0.0F, 1.0F,
    -0.5F, -0.5F, -0.5F, -1.0F,  0.0F,  0.0F,  0.0F, 1.0F,
    -0.5F, -0.5F,  0.5F, -1.0F,  0.0F,  0.0F,  0.0F, 0.0F,
    -0.5F,  0.5F,  0.5F, -1.0F,  0.0F,  0.0F,  1.0F, 0.0F,

     0.5F,  0.5F,  0.5F,  1.0F,  0.0F,  0.0F,  1.0F, 0.0F,
     0.5F,  0.5F, -0.5F,  1.0F,  0.0F,  0.0F,  1.0F, 1.0F,
     0.5F, -0.5F, -0.5F,  1.0F,  0.0F,  0.0F,  0.0F, 1.0F,
     0.5F, -0.5F, -0.5F,  1.0F,  0.0F,  0.0F,  0.0F, 1.0F,
     0.5F, -0.5F,  0.5F,  1.0F,  0.0F,  0.0F,  0.0F, 0.0F,
     0.5F,  0.5F,  0.5F,  1.0F,  0.0F,  0.0F,  1.0F, 0.0F,

    -0.5F, -0.5F, -0.5F,  0.0F, -1.0F,  0.0F,  0.0F, 1.0F,
     0.5F, -0.5F, -0.5F,  0.0F, -1.0F,  0.0F,  1.0F, 1.0F,
     0.5F, -0.5F,  0.5F,  0.0F, -1.0F,  0.0F,  1.0F, 0.0F,
     0.5F, -0.5F,  0.5F,  0.0F, -1.0F,  0.0F,  1.0F, 0.0F,
    -0.5F, -0.5F,  0.5F,  0.0F, -1.0F,  0.0F,  0.0F, 0.0F,
    -0.5F, -0.5F, -0.5F,  0.0F, -1.0F,  0.0F,  0.0F, 1.0F,

    -0.5F,  0.5F, -0.5F,  0.0F,  1.0F,  0.0F,  0.0F, 1.0F,
     0.5F,  0.5F, -0.5F,  0.0F,  1.0F,  0.0F,  1.0F, 1.0F,
     0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,  1.0F, 0.0F,
     0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,  1.0F, 0.0F,
    -0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,  0.0F, 0.0F,
    -0.5F,  0.5F, -0.5F,  0.0F,  1.0F,  0.0F,  0.0F, 1.0F
};
std::vector<glm::vec3> cubePositions = {
    glm::vec3( 0.0F,  0.0F,  0.0F),
    glm::vec3( 2.0F,  5.0F, -15.0F),
    glm::vec3(-1.5F, -2.2F, -2.5F),
    glm::vec3(-3.8F, -2.0F, -12.3F),
    glm::vec3( 2.4F, -0.4F, -3.5F),
    glm::vec3(-1.7F,  3.0F, -7.5F),
    glm::vec3( 1.3F, -2.0F, -2.5F),
    glm::vec3( 1.5F,  2.0F, -2.5F),
    glm::vec3( 1.5F,  0.2F, -1.5F),
    glm::vec3(-1.3F,  1.0F, -1.5F)
};
// clang-format on

int windowWidth = 800;
int windowHeight = 600;

double deltaTime = 0.0F;
double lastFrame = 0.0F;

bool cursorFirstEntering = true;
auto cursorX = float(windowWidth) / 2;
auto cursorY = float(windowHeight) / 2;

Camera cam(glm::vec3(0.0F, 0.0F, 3.0F));

int main() {
    auto* window = init();

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(vertices.size() * sizeof(float)), vertices.data(),
                 GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint lightVao;
    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // shaders
    Shader shader("../shader/container.vs", "../shader/container.fs");
    Shader lightShader("../shader/light.vs", "../shader/light.fs");

    stbi_set_flip_vertically_on_load(true);
    int width;
    int channelCnt;
    int height;

    // textures
    GLuint diffuseMap;
    glGenTextures(1, &diffuseMap);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);  // target-object binding
    unsigned char* img = stbi_load("../img/container2.png", &width, &height, &channelCnt, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(img);
    img = stbi_load("../img/container2_specular.png", &width, &height, &channelCnt, 0);
    GLuint specularMap;
    glGenTextures(1, &specularMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);  // target-object binding
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glm::vec3 objectColor(1.0F, 0.5F, 0.31F);
    glm::vec3 lightColor(1.0F);
    // clang-format off
    std::vector<glm::vec3> lightsInWorld = {
        glm::vec3( 0.7F,  0.2F,  2.0F), 
        glm::vec3( 2.3F, -3.3F, -4.0F), 
        glm::vec3(-4.0F,  2.0F, -12.0F),
        glm::vec3( 0.0F,  0.0F, -3.0F)
    };
    // clang-format on
    shader.use();
    glUniform1i(glGetUniformLocation(shader.id, "lightCnt"), int(lightsInWorld.size()));
    

    glEnable(GL_DEPTH_TEST);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window);

        /* background */
        glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // for (int i = 0; i < lightsInWorld.size(); ++i) {
        //     auto t = float(glfwGetTime());
        //     auto radius = 2.0F;
        //     auto moveSpeed = 2.0F;
        //     lightsInWorld[i].x = radius * std::cos(moveSpeed * t);
        //     lightsInWorld[i].y = 0.0F;
        //     lightsInWorld[i].z = radius * std::sin(moveSpeed * t);
        //     auto m = glm::rotate(glm::mat4(1.0F), glm::radians(360 * std::sin(t / 16.0F)),
        //                         glm::vec3(0.0F, 0.0F, 1.0F));
        //     lightsInWorld[i] = glm::vec3(m * glm::vec4(lightsInWorld[i], 1.0F));

        //     lightColor.r = float(glm::clamp(std::sin(0.5 * t) / 2 + 0.5, 0.2, 1.0));
        //     lightColor.g = float(glm::clamp(std::sin(1.3 * t) / 2 + 0.5, 0.3, 1.0));
        //     lightColor.b = float(glm::clamp(std::sin(2.7 * t) / 2 + 0.5, 0.5, 1.0));
        // }

        glm::mat4 view = cam.viewMatrix();
        glm::mat4 projection = cam.projectionMatrix();

        /* object rendering */
        shader.use();

        glUniformMatrix4fv(glGetUniformLocation(shader.id, "view"), 1, GL_FALSE,
                           glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.id, "projection"), 1, GL_FALSE,
                           glm::value_ptr(projection));

        // glUniform3fv(glGetUniformLocation(shader.id, "lightInWorld"), 1,
        //              glm::value_ptr(lightInWorld));

        auto mAmbient = glm::vec3(1.0F, 0.5F, 0.31F);
        auto mDiffuse = glm::vec3(1.0F, 0.5F, 0.31F);
        auto mSpecular = glm::vec3(0.5F, 0.5F, 0.5F);
        auto mShininess = 32.0F;

        glUniform1i(glGetUniformLocation(shader.id, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(shader.id, "material.specular"), 1);
        glUniform1f(glGetUniformLocation(shader.id, "material.shininess"), mShininess);

        // auto lDiffuse = glm::vec3(0.5F, 0.5F, 0.5F);
        glm::vec3 lDiffuse = lightColor;
        auto      lAmbient = glm::vec3(0.2F);
        auto      lSpecular = glm::vec3(1.0F, 1.0F, 1.0F);

        for (int i = 0; i < lightsInWorld.size(); ++i) {
            glUniform3fv(
                glGetUniformLocation(shader.id, ("lights[" + std::to_string(i) + "].pos").c_str()),
                1, glm::value_ptr(lightsInWorld[i]));
            glUniform3fv(glGetUniformLocation(
                             shader.id, ("lights[" + std::to_string(i) + "].ambient").c_str()),
                         1, glm::value_ptr(lAmbient));
            glUniform3fv(glGetUniformLocation(
                             shader.id, ("lights[" + std::to_string(i) + "].diffuse").c_str()),
                         1, glm::value_ptr(lDiffuse));
            glUniform3fv(glGetUniformLocation(
                             shader.id, ("lights[" + std::to_string(i) + "].specular").c_str()),
                         1, glm::value_ptr(lSpecular));
        }

        for (int i = 0; i < cubePositions.size(); ++i) {
            auto model = glm::mat4(1.0F);
            model = glm::translate(model, cubePositions[i]);
            auto angle = 20.0F * float(i);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0F, 0.3F, 0.5F));

            glm::mat3 normalMatrix = glm::transpose(glm::inverse(model));

            glUniformMatrix4fv(glGetUniformLocation(shader.id, "model"), 1, GL_FALSE,
                               glm::value_ptr(model));

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        /* light rendering */
        lightShader.use();

        for (int i = 0; i < lightsInWorld.size(); ++i) {
            glm::mat4 lightModel(1.0F);
            lightModel = glm::translate(lightModel, lightsInWorld[i]);
            lightModel = glm::scale(lightModel, glm::vec3(0.2F));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.id, "model"), 1, GL_FALSE,
                               glm::value_ptr(lightModel));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.id, "view"), 1, GL_FALSE,
                               glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.id, "projection"), 1, GL_FALSE,
                               glm::value_ptr(projection));

            glUniform3fv(glGetUniformLocation(lightShader.id, "lightColor"), 1,
                         glm::value_ptr(lightColor));

            glBindVertexArray(lightVao);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
        cursorX = float(xPos);
        cursorY = float(yPos);
        cursorFirstEntering = false;
    }
    auto dx = xPos - cursorX;
    auto dy = cursorY - yPos;
    cursorX = float(xPos);
    cursorY = float(yPos);
    cam.processCursor(dx, dy);
}

void scrollCallback(GLFWwindow* window, double dx, double dy) {
    cam.processScroll(dy);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { cam.moveForward(deltaTime); }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { cam.moveBackward(deltaTime); }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { cam.moveLeft(deltaTime); }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { cam.moveRight(deltaTime); }
}

GLFWwindow* init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lighting", nullptr, nullptr);
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

    return window;
}
