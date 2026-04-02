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
            this->models = models;

            auto *data = new float[8 * numVertices];
            for (int i = 0; i < numVertices; i++)
            {
                const Vertex v = object.vertices.at(i);
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
