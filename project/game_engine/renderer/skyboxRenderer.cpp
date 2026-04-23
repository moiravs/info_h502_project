#include "skyboxRenderer.h"

#include "../../utils/constants.h"
#include "../displaymanager.h"
#include "../mainCamera.h"

SkyboxRenderer::SkyboxRenderer(Skybox *skybox)
    : Renderer(this->generateShader("skybox")), _transparent(false), _skybox(skybox)
{
    cubemapTexture = skybox->getTextureId();

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, skybox->getVertices().size() * sizeof(float), skybox->getVertices().data(),
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    this->_shader->setInteger("skybox", 0);
}

void SkyboxRenderer::updateUniforms() const
{
    const auto camera = MainCamera::get();
    const glm::mat4 projection = glm::perspective(glm::radians(camera->getZoom()),
                                                  static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 10000.0f);
    const auto view = glm::mat4(glm::mat3(camera->getViewMatrix())); // remove translation from the view matrix
    _shader->setMatrix4("view", view);
    _shader->setMatrix4("projection", projection);
}

void SkyboxRenderer::render()
{
    Renderer::render();

    glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
    // skybox cube
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}
