#include "mesh.h"

void Mesh::setupMesh() {
    // generate buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // bind buffers
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // send buffer data of vbo & ebo
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(vertices.size() * sizeof(Vertex)),
                 vertices.data(), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 GLsizeiptr(indices.size() * sizeof(GLuint)), indices.data(),
                 GL_DYNAMIC_DRAW);

    // setting vao attributes
    // vertex position: 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    // vertex normals: 1
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, normal));
    // vertex texture coordinates: 2
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, texCoord));

    // unbind vao
    glBindVertexArray(0);
}

void Mesh::draw(const Shader& shader) const {
    auto nextDiffuseSuffix = 0U;
    auto nextSpecularSuffix = 0U;

    // iterate through textures (vector of Texture), active texture units and
    // link to proper shader uniform variables of different texture types
    // assume uniform vars in the form: material.<type><suffix>
    // e.g. material.diffuse0, material.specular1
    for (auto i = 0; i < textures.size(); ++i) {
        // active i-th texture unit
        glActiveTexture(GL_TEXTURE0 + i);

        // find type & suffix for this texture
        auto suffix = 0U;
        auto type = textures[i].type;
        if (type == "diffuse")
            suffix = nextDiffuseSuffix++;
        else if (type == "specular")
            suffix = nextSpecularSuffix++;

        std::string name = type + std::to_string(suffix);
        const char* varname = name.c_str();
        // bind sampler in shader to i-th texture unit
        glUniform1i(glGetUniformLocation(shader.id, varname), i);
        // bind i-th texture unit to i-th texture object
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // draw mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_INT,
                   nullptr);
    glBindVertexArray(0);
}
