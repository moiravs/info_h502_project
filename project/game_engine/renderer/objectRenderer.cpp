#include "objectRenderer.h"

#include "../manager/displaymanager.h"

ObjectRenderer::ObjectRenderer() : Renderer(this->generateShader("lamp")) {}

void ObjectRenderer::registerObject(const std::shared_ptr<Object> object)
{
    this->Renderer::registerObject(object);
    const Mesh &mesh = object->getMesh();

    _vaos.resize(mesh.m_Entries.size());
    glGenVertexArrays(_vaos.size(), _vaos.data());

    for (unsigned int i = 0; i < mesh.m_Entries.size(); i++)
    {
        glBindVertexArray(_vaos[i]);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.m_Entries[i].VB);

        constexpr GLsizei stride = sizeof(Vertex);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Texture));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, Normal));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_Entries[i].IB);
    }
    glBindVertexArray(0);
}

void ObjectRenderer::updateUniforms() const
{
    this->_shader->setVector3f("materialColour", glm::vec3(1.0f, 1.0, 1.0));
    this->_shader->setMatrix4("model", this->_object->getModel());
    const glm::mat4 inverseModel = glm::transpose(glm::inverse(this->_object->getModel()));
    this->_shader->setMatrix4("itM", inverseModel);
}

void ObjectRenderer::render()
{
    Renderer::render();

    const Mesh &mesh = _object->getMesh();

    for (unsigned int i = 0; i < _vaos.size(); i++)
    {
        const auto &entry = mesh.m_Entries[i];

        if (entry.MaterialIndex < mesh.m_Textures.size() && mesh.m_Textures[entry.MaterialIndex])
        {
            mesh.m_Textures[entry.MaterialIndex]->bind();
        }

        glBindVertexArray(_vaos[i]);
        glDrawElements(GL_TRIANGLES, entry.NumIndices, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}
