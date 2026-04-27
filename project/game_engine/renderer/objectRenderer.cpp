#include "objectRenderer.h"

#include "../../utils/utils.h"
#include "../manager/displaymanager.h"
#include "../entity/object.h"

ObjectRenderer::ObjectRenderer(const std::string &shaderName) : MeshRenderer(shaderName)
{
}

void ObjectRenderer::updateUniforms() const
{
    this->_shader->setVector3f("materialColour", this->getEntity<Object>()->getColor());
    this->_shader->setMatrix4("model", this->_entity->getModel());
    const glm::mat4 inverseModel = glm::transpose(glm::inverse(this->_entity->getModel()));
    this->_shader->setMatrix4("itM", inverseModel);

    _shader->use();
    glUniform1i(glGetUniformLocation(_shader->getID(), "grassTex"), 1);
    glUniform1i(glGetUniformLocation(_shader->getID(), "rockTex"), 2);
    glUniform1i(glGetUniformLocation(_shader->getID(), "snowTex"), 3);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grassTex.getTexture());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, rockTex.getTexture());

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, snowTex.getTexture());

    // 3. Reset Active Texture to 0 so other code doesn't get confused
    glActiveTexture(GL_TEXTURE0);
}
