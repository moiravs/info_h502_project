#include "depthMap.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "../../utils/utils.h"

DepthMap::DepthMap(const std::shared_ptr<DepthCam> &depthCam): _depthCam(depthCam)
{
    this->depthShader = std::make_shared<Shader>(
        PATH_TO_SRC "/../assets/shaders/depth.vert",
        PATH_TO_SRC "/../assets/shaders/depth.frag"
        );

    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 DEPTH_WIDTH, DEPTH_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    constexpr float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthMap::loadModel(const glm::mat4 &model) const
{
    this->depthShader->use();
    this->depthShader->setMatrix4("model", model);
}

void DepthMap::prepare() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glViewport(0, 0, DEPTH_WIDTH, DEPTH_HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT);
    this->depthShader->setMatrix4("PV", this->_depthCam->getPV());
}

void DepthMap::cleanup() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK); // Reactivate color drawing
    glReadBuffer(GL_BACK);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

GLuint DepthMap::getTexture() const
{
    return this->depthMapTexture;
}
