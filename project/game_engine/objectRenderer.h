#ifndef ObjectRenderer_H
#define ObjectRenderer_H

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

class ObjectRenderer
{
public:
    GLuint VBO, VAO;
    bool transparent;
    std::vector<glm::mat4> models;
    Shader shader;
    Texture tex;
    std::vector<Object> objects;
    glm::mat4 model = glm::mat4(1.0);

    ObjectRenderer(Shader shader, std::vector<Object> objects, Texture tex) : shader(shader), tex(tex), objects(objects)
    {

        for (auto object : objects)
        {

            tex = Texture(PATH_TO_SRC "/../assets/textures/wall.jpg");
            int numVertices = object.numVertices;

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, object.vertices.size() * sizeof(Vertex), object.vertices.data(), GL_STATIC_DRAW);

            GLsizei stride = sizeof(Vertex);

            auto att_pos = glGetAttribLocation(shader.ID, "position");
            glEnableVertexAttribArray(att_pos);
            glVertexAttribPointer(att_pos, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Position));

            auto att_tex = glGetAttribLocation(shader.ID, "tex_coord");
            glEnableVertexAttribArray(att_tex);
            glVertexAttribPointer(att_tex, 2, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Texture));

            auto att_nor = glGetAttribLocation(shader.ID, "normal");
            glEnableVertexAttribArray(att_nor);
            glVertexAttribPointer(att_nor, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Normal));

            glBindVertexArray(0);
        }
    }

    void draw() const
    {
    }

    void render()
    {

        tex.bind();
        for (auto obj : objects)
        {
            this->shader.use();
            this->shader.setMatrix4("model", obj.model);
            this->shader.updatePos(camera);
            if (transparent)
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(obj.numVertices));
        }
        tex.unbind();
    }
};
#endif
