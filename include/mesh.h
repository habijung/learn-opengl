//
// Created by habi on 1/13/2023.
//

#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "shaders/shader.h"


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
};


class Mesh {
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();
    }

    void Draw(Shader &shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;

        for (unsigned int i = 0; i < textures.size(); i++) {
            // Activate proper texture unit before binding
            glActiveTexture(GL_TEXTURE0 + 1);

            // Retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;

            if (name == "texture_diffuse") {
                number = std::to_string(diffuseNr++);
            } else if (name == "texture_specular") {
                number = std::to_string(specularNr++);
            }
        }
        glActiveTexture(GL_TEXTURE0);

        // Draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    // render data
    unsigned int VAO, VBO, EBO;

    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }
};


#endif //MESH_H
