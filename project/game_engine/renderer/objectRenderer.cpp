#include "objectRenderer.h"

#include "../manager/displaymanager.h"
#include "../entity/object.h"
#include "../../utils/constants.h"

std::shared_ptr<Texture> ObjectRenderer::grassTex = nullptr;
std::shared_ptr<Texture> ObjectRenderer::rockTex = nullptr;
std::shared_ptr<Texture> ObjectRenderer::snowTex = nullptr;

ObjectRenderer::ObjectRenderer(const std::string &shaderName) : MeshRenderer(shaderName)
{
    if (ObjectRenderer::grassTex == nullptr)
    {
        grassTex = std::make_shared<Texture>(PATH_TO_SRC "/../assets/textures/terrain/grass.jpg");
        rockTex = std::make_shared<Texture>(PATH_TO_SRC "/../assets/textures/terrain/rock.jpg");
        snowTex = std::make_shared<Texture>(PATH_TO_SRC "/../assets/textures/terrain/snow.jpg");
    }
}

ObjectRenderer::ObjectRenderer(const std::shared_ptr<Shader>& shader) : MeshRenderer(shader)
{
}

void ObjectRenderer::updateUniforms() const
{
    MeshRenderer::updateUniforms();
    this->_shader->setVector3f("materialColour", this->getEntity<Object>()->getColor());
    this->_shader->setMatrix4("model", this->_entity->getModel());
    this->_shader->setVector2f("material", this->getEntity<Object>()->getMaterial());
    const glm::mat4 inverseModel = glm::transpose(glm::inverse(this->_entity->getModel()));
    this->_shader->setMatrix4("itM", inverseModel);

    _shader->use();
    glUniform1i(glGetUniformLocation(_shader->getID(), "grassTex"), GRASS_TEX_IDX);
    glUniform1i(glGetUniformLocation(_shader->getID(), "rockTex"), ROCK_TEX_IDX);
    glUniform1i(glGetUniformLocation(_shader->getID(), "snowTex"), SNOW_TEX_IDX);

    glActiveTexture(GRASS_TEX);
    glBindTexture(GL_TEXTURE_2D, grassTex->getTexture());

    glActiveTexture(ROCK_TEX);
    glBindTexture(GL_TEXTURE_2D, rockTex->getTexture());

    glActiveTexture(SNOW_TEX);
    glBindTexture(GL_TEXTURE_2D, snowTex->getTexture());

    // 3. Reset Active Texture to 0 so other code doesn't get confused
    glActiveTexture(GL_TEXTURE0);
}
