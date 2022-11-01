#pragma once

#include <assimp/material.h>
#include <assimp/scene.h>

#include <map>

#include "mesh.h"

class Model {
  public:
    Model(const std::string& path, glm::vec3 pos = glm::vec3(0.0f),
          glm::vec3 scale = glm::vec3(1.0f))
        : pos(pos), scale(scale) {
        loadModel(path);
    }

    glm::vec3 pos;
    glm::vec3 scale;
    glm::mat4 modelMatrix() const;

    void draw(const Shader& shader) const;

  private:
    // a model comprises multiple meshes
    std::vector<Mesh> meshes;
    // the directory (path to folder) of model file, it will be used to load
    // texture maps
    std::string directory;
    // all the textures that have been loaded
    std::map<std::string, Texture> texturesLoaded;

    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadTextureFromMaterial(aiMaterial* material,
                                                 aiTextureType aiTexType,
                                                 const std::string& type);
    static GLuint generateTextureFromFile(const char* path,
                                          const std::string& directory);
};