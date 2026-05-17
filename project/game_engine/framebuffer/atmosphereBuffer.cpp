
#include "atmosphereBuffer.h"

#include "../../utils/utils.h"
#include "../../utils/constants.h"

AtmosphereFrameBuffer::AtmosphereFrameBuffer(const int width, const int height, const GLuint depthTexture) : FrameBuffer(width, height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
}

void AtmosphereFrameBuffer::bindTextures()
{
    glActiveTexture(ATM_COLOR_TEX);
    glBindTexture(GL_TEXTURE_2D, this->_tex);
}

void AtmosphereFrameBuffer::createTextures()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
    createColorTexture(this->_tex, GL_COLOR_ATTACHMENT0);

    constexpr GLuint attachments[] =
        {
            GL_COLOR_ATTACHMENT0};

    glDrawBuffers(1, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ERROR("Failed building the FBO.");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AtmosphereFrameBuffer::begin()
{
    FrameBuffer::begin();
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
}

void AtmosphereFrameBuffer::end()
{
    FrameBuffer::end();
}

GLuint AtmosphereFrameBuffer::getTexture() const
{
    return this->_tex;
}
