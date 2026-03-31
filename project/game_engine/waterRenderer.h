#ifndef WaterRenderer_H
#define WaterRenderer_H

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
#include "shader.h"

class WaterRenderer
{
public:
    std::vector<Vertex> vertices;

    size_t numVertices;

    GLuint VBO, VAO;
    bool transparent;
    std::vector<glm::mat4> models;
    Shader shader;
    Object object;

    WaterRenderer(Shader shader, Object object, std::vector<glm::mat4> models) : shader(shader), object(object), models(models)
    {
        vertices = object.getVertices();
        this->models = models;
        numVertices = vertices.size();

        auto *data = new float[8 * numVertices];
        for (int i = 0; i < numVertices; i++)
        {
            const Vertex v = vertices.at(i);
            data[i * 8] = v.Position.x;
            data[i * 8 + 1] = v.Position.y;
            data[i * 8 + 2] = v.Position.z;

            data[i * 8 + 3] = v.Texture.x;
            data[i * 8 + 4] = v.Texture.y;

            data[i * 8 + 5] = v.Normal.x;
            data[i * 8 + 6] = v.Normal.y;
            data[i * 8 + 7] = v.Normal.z;
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // define VBO and VAO as active buffer and active vertex array
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<long>(sizeof(Vertex) * numVertices), data, GL_STATIC_DRAW);

        auto att_pos = glGetAttribLocation(shader.ID, "position");
        glEnableVertexAttribArray(att_pos);
        glVertexAttribPointer(att_pos, 3, GL_FLOAT, false, 8 * sizeof(float), nullptr);

        auto att_col = glGetAttribLocation(shader.ID, "normal");
        glEnableVertexAttribArray(att_col);
        glVertexAttribPointer(att_col, 3, GL_FLOAT, false, 8 * sizeof(float),
                              reinterpret_cast<void *>(5 * sizeof(float)));

        auto att_tex = glGetAttribLocation(shader.ID, "tex_coord");
        glEnableVertexAttribArray(att_tex);
        glVertexAttribPointer(att_tex, 2, GL_FLOAT, false, 8 * sizeof(float),
                              reinterpret_cast<void *>(3 * sizeof(float)));

        // desactive the buffer
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        delete[] data;
    }

    void draw() const
    {

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(numVertices));
    }

    void render()
    {

        this->shader.use();
        this->shader.updatePos(camera);
        this->draw();
    }
};
#endif
