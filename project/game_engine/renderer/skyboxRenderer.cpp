#include "skyboxRenderer.h"

SkyboxRenderer::SkyboxRenderer(Skybox *skybox)
    : Renderer(this->generateShader("skybox")), _transparent(false), _skybox(skybox)
{
    cubemapTexture = skybox->getTextureId();

    this->createVAOs(1);
    this->createVBOs(1);
    glBindVertexArray(_VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, _VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, skybox->getVertices().size() * sizeof(float), skybox->getVertices().data(),
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    this->_shader->setInteger("skybox", 0);
}

void SkyboxRenderer::updateUniforms() const
{}

void SkyboxRenderer::render()
{
    Renderer::render();

    glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
    // skybox cube
    glBindVertexArray(_VAOs[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}
