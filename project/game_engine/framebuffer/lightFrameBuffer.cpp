
#include "lightFrameBuffer.h"

#include "../../utils/utils.h"

LightFrameBuffer::LightFrameBuffer(const int width, const int height, const GLuint depthTexture): FrameBuffer(width, height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
}

void LightFrameBuffer::bindTextures()
{
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, this->_tex);
}

void LightFrameBuffer::createTextures()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
    createColorTexture(this->_tex, GL_COLOR_ATTACHMENT0);

    constexpr GLuint attachments[] =
    {
        GL_COLOR_ATTACHMENT0
    };

    glDrawBuffers(1, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ERROR("Failed building the FBO.");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightFrameBuffer::begin()
{
    FrameBuffer::begin();
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
}

void LightFrameBuffer::end()
{
    FrameBuffer::end();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLuint LightFrameBuffer::getTexture() const
{
    return this->_tex;
}
