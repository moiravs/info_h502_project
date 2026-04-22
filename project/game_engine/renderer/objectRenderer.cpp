#include "objectRenderer.h"

#include "../displaymanager.h"
#include "../mainCamera.h"

ObjectRenderer::ObjectRenderer(std::shared_ptr<Shader> shader) : Renderer(std::move(shader)) {}

void ObjectRenderer::registerObject(const std::shared_ptr<Object> object)
{
    this->Renderer::registerObject(object);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, object->getNumVertices() * sizeof(Vertex), object->getVertices().data(), GL_STATIC_DRAW);

    constexpr GLsizei stride = sizeof(Vertex);

    const auto att_pos = glGetAttribLocation(_shader->getID(), "position");
    glEnableVertexAttribArray(att_pos);
    glVertexAttribPointer(att_pos, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, Position)));

    const auto att_tex = glGetAttribLocation(_shader->getID(), "tex_coord");
    glEnableVertexAttribArray(att_tex);
    glVertexAttribPointer(att_tex, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, Texture)));
    const auto att_nor = glGetAttribLocation(_shader->getID(), "normal");
    glEnableVertexAttribArray(att_nor);
    glVertexAttribPointer(att_nor, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, Normal)));

    glBindVertexArray(0);
}

void ObjectRenderer::render()
{
    this->_shader->setMatrix4("model", this->_object->getModel());
    const glm::mat4 inverseModel = glm::transpose(glm::inverse(this->_object->getModel()));
    this->_shader->setMatrix4("itM", inverseModel);
    this->_shader->updatePos(MainCamera::get());
    if (_transparent)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(this->_object->getNumVertices()));
}
