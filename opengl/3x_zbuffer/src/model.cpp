#include "model.h"

// clang-format off
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <utility>
#include "glm/ext/matrix_transform.hpp"
// clang-format on

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static std::vector<int> channelEnum{-1, GL_RED, -1, GL_RGB, GL_RGBA};

glm::mat4 Model::modelMatrix() const {
    glm::mat4 m = glm::translate(glm::mat4(1.0f), pos);
    m = glm::scale(m, scale);
    return m;
}

void Model::draw(const Shader& shader) const {
    for (const auto& m : meshes) m.draw(shader);
}


void Model::loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
    }

    directory = path.substr(0, path.find_last_of('/'));

    // recursively process all nodes' meshes
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (auto i = 0U; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (auto i = 0U; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    // process mesh vertices
    std::vector<Vertex> vertices;
    for (auto i = 0U; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.pos = {mesh->mVertices[i].x, mesh->mVertices[i].y,
                      mesh->mVertices[i].z};
        vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y,
                         mesh->mNormals[i].z};

        // if mesh contains texture coordinates
        if (mesh->mTextureCoords[0]) {
            vertex.texCoord = {mesh->mTextureCoords[0][i].x,
                               mesh->mTextureCoords[0][i].y};
        }
        vertices.push_back(vertex);
    }

    // process mesh indices
    std::vector<GLuint> indices;
    for (auto i = 0U; i < mesh->mNumFaces; ++i) {
        auto face = mesh->mFaces[i];
        for (auto j = 0U; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process mesh materials (if contains)
    std::vector<Texture> textures;
    if (mesh->mMaterialIndex >= 0) {
        auto* material = scene->mMaterials[mesh->mMaterialIndex];
        // load diffuse material
        std::vector<Texture> diffuseMaps =
            loadTextureFromMaterial(material, aiTextureType_DIFFUSE, "diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // load specular material
        std::vector<Texture> specularMaps = loadTextureFromMaterial(
            material, aiTextureType_SPECULAR, "specular");
        textures.insert(textures.end(), specularMaps.begin(),
                        specularMaps.end());
    }

    return {vertices, indices, textures};
}

std::vector<Texture> Model::loadTextureFromMaterial(aiMaterial* material,
                                                    aiTextureType aiTexType,
                                                    const std::string& type) {
    std::vector<Texture> textures;
    for (auto i = 0U; i < material->GetTextureCount(aiTexType); ++i) {
        // retrieve texture path
        aiString aiPath;
        material->GetTexture(aiTexType, i, &aiPath);
        std::string path(aiPath.C_Str());

        // check if the texture has been loaded already
        // if so, skip loading and use previously loaded texture
        // else, generate a new texture and store it
        if (texturesLoaded.count(path)) {
            textures.push_back(texturesLoaded.at(path));
        } else {
            Texture tex;
            tex.id = generateTextureFromFile(path.c_str(), directory);
            tex.type = type;
            textures.emplace_back(tex);
            texturesLoaded.insert(std::make_pair(path, tex));
        }
    }
    return textures;
}

GLuint Model::generateTextureFromFile(const char* path,
                                      const std::string& directory) {
    auto filename = std::string(path);
    filename = directory + '/' + filename;

    GLuint tex;
    glGenTextures(1, &tex);

    // load image
    int width;
    int height;
    int channelCount;
    auto* img = stbi_load(filename.c_str(), &width, &height, &channelCount, 0);
    if (!img) {
        std::cout << "Failed to load image at path: " << filename << "\n";
        stbi_image_free(img);
        return tex;
    }

    // channelCount:
    // 1: GL_RED  3: GL_RGB  4: GL_RGBA
    GLenum format = channelEnum[channelCount];

    // send to texture object
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, int(format), width, height, 0, format,
                 GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(GL_TEXTURE_2D);

    // set texture wrapping and filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(img);
    return tex;
}