#include "depthMapFrameBuffer.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "../../utils/utils.h"

DepthMapFrameBuffer::DepthMapFrameBuffer(const int width, const int height, const std::shared_ptr<DepthCam> &depthCam) :
    FrameBuffer(width, height), _depthCam(depthCam), _tex(0)
{
    this->_shader = std::make_shared<Shader>(PATH_TO_SRC "/../assets/shaders/depth.vert",
                                                 PATH_TO_SRC "/../assets/shaders/depth.frag");
}

void DepthMapFrameBuffer::createTextures()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
    this->createDepthTexture(this->_tex);

    constexpr float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthMapFrameBuffer::begin()
{
    this->FrameBuffer::begin();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);
    this->_shader->setMatrix4("PV", this->_depthCam->getPV());
}

void DepthMapFrameBuffer::end()
{
    this->FrameBuffer::end();
    glDrawBuffer(GL_BACK); // Reactivate color drawing
    glReadBuffer(GL_BACK);
}

void DepthMapFrameBuffer::bindTextures()
{
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, this->_tex);
}

GLuint DepthMapFrameBuffer::getTexture() const
{
    return this->_tex;
}

std::shared_ptr<Shader> DepthMapFrameBuffer::getShader() const
{
    return this->_shader;
}
