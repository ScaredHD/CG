#pragma once
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "shader.h"

struct Vertex {
    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec3 normal = glm::vec3(0.0f);
    glm::vec2 texCoord = glm::vec2(0.0f);
};

struct Texture {
    // id of generated texture
    GLuint id;
    // texture type, e.g. "diffuse", "specular"
    std::string type;
};

class Mesh {
  public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    Mesh() = default;
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices,
         std::vector<Texture> textures)
        : vertices(std::move(vertices)),
          indices(std::move(indices)),
          textures(std::move(textures)) {
        setupMesh();
    }

    void draw(const Shader& shader) const;

  private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    void setupMesh();
};
