#ifndef InstancedRenderer_H
#define InstancedRenderer_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "object.h"
#include "texture.h"
#include "shader.h"

class InstancedRenderer
{
public:
    GLuint VAO, VBO, instanceVBO;
    int vertexCount;
    int instanceCount;
    Shader shader;
    Texture tex;

    InstancedRenderer(Shader s, Object &model, Texture t, std::vector<glm::mat4> &matrices)
        : shader(s), tex(t)
    {
        tex = Texture(PATH_TO_SRC "/../assets/textures/wall.jpg");

        instanceCount = matrices.size();
        vertexCount = model.numVertices;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &instanceVBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(Vertex), model.vertices.data(), GL_STATIC_DRAW);

        // Position (Loc 0)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        // TexCoord (Loc 1)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Texture));
        // Normal (Loc 2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

        // 2. Setup Instance Data (Matrices)
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, instanceCount * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);

        // A mat4 is 4 vec4s. We enable locations 3, 4, 5, and 6.
        for (unsigned int i = 0; i < 4; i++)
        {
            glEnableVertexAttribArray(3 + i);
            glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4) * i));
            // Key: tell OpenGL to update this attribute per INSTANCE
            glVertexAttribDivisor(3 + i, 1);
        }

        glBindVertexArray(0);
    }

    void draw() const
    {
    }

    void render()
    {
        tex.bind();

        shader.use();
        shader.updatePos(camera);

        glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, instanceCount);
        glBindVertexArray(0);
        tex.unbind();
    }
};
#endif
