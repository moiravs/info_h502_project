#include "instanceRenderer.h"

#include "../displaymanager.h"
#include "../mainCamera.h"

InstancedRenderer::InstancedRenderer(std::shared_ptr<Shader> shader, std::shared_ptr<Object> model, Texture* texture, const std::vector<glm::mat4>& matrices)
: Renderer(std::move(shader)), _tex(texture)
{
    _instanceCount = matrices.size();
    _vertexCount = model->getNumVertices();

    glGenBuffers(1, &_instanceVBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, model->getNumVertices() * sizeof(Vertex), model->getVertices().data(), GL_STATIC_DRAW);

    // Position (Loc 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    // TexCoord (Loc 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Texture));
    // Normal (Loc 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

    // 2. Setup Instance Data (Matrices)
    glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, _instanceCount * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);

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

void InstancedRenderer::render()
{
    _tex->bind();

    _shader->updatePos(MainCamera::get());

    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, _vertexCount, _instanceCount);
    glBindVertexArray(0);
    Texture::unbind();
}

InstancedRenderer::~InstancedRenderer()
{
    Renderer::~Renderer();
    glDeleteBuffers(1, &_instanceVBO);
}
