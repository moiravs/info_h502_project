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
#include "renderer.h"

class ObjectRenderer : public Renderer
{
public:
    GLuint _VBO, _VAO;
    bool _transparent;
    std::vector<glm::mat4> _models;
    Shader _shader;
    Texture _tex;
    std::vector<Object *> _objects;
    glm::mat4 model = glm::mat4(1.0);

    ObjectRenderer(Shader shader, Object *object) : _shader(shader)
    {
        this->_objects.push_back(object);
        initObjectRenderer();
    }

    ObjectRenderer(Shader shader, Object *object, Texture tex) : _shader(shader), _tex(tex)
    {
        this->_objects.push_back(object);
        initObjectRenderer();
    }

    ObjectRenderer(Shader shader, std::vector<Object *> objects, Texture tex) : _shader(shader), _tex(tex), _objects(objects)
    {
        initObjectRenderer();
    }

    void initObjectRenderer()
    {

        for (auto object : _objects)
        {

            int numVertices = object->numVertices;

            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);

            glBindVertexArray(_VAO);
            glBindBuffer(GL_ARRAY_BUFFER, _VBO);

            glBufferData(GL_ARRAY_BUFFER, object->vertices.size() * sizeof(Vertex), object->vertices.data(), GL_STATIC_DRAW);

            GLsizei stride = sizeof(Vertex);

            auto att_pos = glGetAttribLocation(_shader.ID, "position");
            glEnableVertexAttribArray(att_pos);
            glVertexAttribPointer(att_pos, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Position));

            auto att_tex = glGetAttribLocation(_shader.ID, "tex_coord");
            glEnableVertexAttribArray(att_tex);
            glVertexAttribPointer(att_tex, 2, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Texture));
            auto att_nor = glGetAttribLocation(_shader.ID, "normal");
            glEnableVertexAttribArray(att_nor);
            glVertexAttribPointer(att_nor, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Normal));

            glBindVertexArray(0);
        }
    }

    void render() override
    {

        for (auto obj : _objects)
        {
            this->_shader.use();
            this->_shader.setMatrix4("model", obj->model);
            glm::mat4 inverseModel = glm::transpose(glm::inverse(model));
            this->_shader.setMatrix4("itM", inverseModel);
            this->_shader.updatePos(camera);
            if (_transparent)
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            glBindVertexArray(this->_VAO);
            glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(obj->numVertices));
        }
    }
};
#endif
