#include "objectRenderer.h"

#include "../displaymanager.h"

ObjectRenderer::ObjectRenderer(const Shader& shader, Object* object) : _shader(shader)
{
    this->_objects.push_back(object);
    initObjectRenderer();
}

ObjectRenderer::ObjectRenderer(const Shader& shader, Object* object, const Texture& tex): _shader(shader), _tex(tex)
{
    this->_objects.push_back(object);
    initObjectRenderer();
}

ObjectRenderer::ObjectRenderer(const Shader& shader, const std::vector<Object*>& objects, const Texture& tex) : _shader(shader), _tex(tex), _objects(objects)
{
    initObjectRenderer();
}

void ObjectRenderer::initObjectRenderer()
{
    for (auto object : _objects)
    {

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, object->getNumVertices() * sizeof(Vertex), object->getVertices().data(), GL_STATIC_DRAW);

        GLsizei stride = sizeof(Vertex);

        auto att_pos = glGetAttribLocation(_shader.getID(), "position");
        glEnableVertexAttribArray(att_pos);
        glVertexAttribPointer(att_pos, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Position));

        auto att_tex = glGetAttribLocation(_shader.getID(), "tex_coord");
        glEnableVertexAttribArray(att_tex);
        glVertexAttribPointer(att_tex, 2, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Texture));
        auto att_nor = glGetAttribLocation(_shader.getID(), "normal");
        glEnableVertexAttribArray(att_nor);
        glVertexAttribPointer(att_nor, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Normal));

        glBindVertexArray(0);
    }
}

void ObjectRenderer::render()
{
    for (const auto obj : _objects)
    {
        this->_shader.use();
        this->_shader.setMatrix4("model", obj->getModel());
        glm::mat4 inverseModel = glm::transpose(glm::inverse(model));
        this->_shader.setMatrix4("itM", inverseModel);
        this->_shader.updatePos(camera);
        if (_transparent)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(obj->getNumVertices()));
    }
}
