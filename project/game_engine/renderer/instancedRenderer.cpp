#include "instancedRenderer.h"

#include "../manager/displaymanager.h"
#include "../entity/object.h"

InstancedRenderer::InstancedRenderer(std::shared_ptr<Object> model, Texture *texture, const std::vector<glm::mat4> &matrices)
    : Renderer(this->generateShader("tree")), _model(model), _tex(texture)
{
    _instanceCount = matrices.size();
    const Mesh &mesh = model->getMesh();
    _vaos.resize(mesh.m_Entries.size());
    glGenVertexArrays(_vaos.size(), _vaos.data());

    glGenBuffers(1, &_instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, _instanceCount * sizeof(glm::mat4), matrices.data(), GL_STATIC_DRAW);

    for (unsigned int i = 0; i < mesh.m_Entries.size(); i++)
    {
        glBindVertexArray(_vaos[i]);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.m_Entries[i].VB);

        // Position (Loc 0)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position));

        // TexCoord (Loc 1)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Texture));

        // Normal (Loc 2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_Entries[i].IB);

        glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);
        for (unsigned int j = 0; j < 4; j++)
        {
            glEnableVertexAttribArray(3 + j);
            glVertexAttribPointer(3 + j, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4) * j));
            glVertexAttribDivisor(3 + j, 1);
        }
    }
    glBindVertexArray(0);
}

void InstancedRenderer::render()
{
    Renderer::render(); // Updates shader uniforms
    const Mesh &mesh = _model->getMesh();

    for (unsigned int i = 0; i < _vaos.size(); i++)
    {
        const auto &entry = mesh.m_Entries[i];

        // Use texture from Assimp material, fallback to constructor texture if null
        if (entry.MaterialIndex < mesh.m_Textures.size() && mesh.m_Textures[entry.MaterialIndex])
        {
            mesh.m_Textures[entry.MaterialIndex]->bind();
        }
        else if (_tex)
        {
            _tex->bind();
        }

        glBindVertexArray(_vaos[i]);
        // Use glDrawElementsInstanced for indexed Assimp data
        glDrawElementsInstanced(GL_TRIANGLES, entry.NumIndices, GL_UNSIGNED_INT, 0, _instanceCount);
    }

    glBindVertexArray(0);
    Texture::unbind();
}

void InstancedRenderer::updateUniforms() const
{}

InstancedRenderer::~InstancedRenderer()
{
    if (!_vaos.empty())
        glDeleteVertexArrays(_vaos.size(), _vaos.data());
    glDeleteBuffers(1, &_instanceVBO);
}
