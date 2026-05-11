
#include "refractionFrameBuffer.h"

#include "../../utils/utils.h"
#include "../../utils/constants.h"

RefractionFrameBuffer::RefractionFrameBuffer(const int width, const int height) :
    FrameBuffer(width, height)
{}

void RefractionFrameBuffer::createTextures()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
    createColorTexture(this->_refractionTexture, GL_COLOR_ATTACHMENT0);
    createDepthBuffer(this->_refractionDepthTexture);

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

void RefractionFrameBuffer::bindTextures()
{
    glActiveTexture(REFRACTION_TEX);
    glBindTexture(GL_TEXTURE_2D, this->_refractionTexture);
}

void RefractionFrameBuffer::begin()
{
    FrameBuffer::begin();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CLIP_DISTANCE0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RefractionFrameBuffer::end()
{
    FrameBuffer::end();
    glDisable(GL_CLIP_DISTANCE0);
}

RefractionFrameBuffer::~RefractionFrameBuffer()
{
    glDeleteTextures(1, &this->_refractionTexture);
    glDeleteRenderbuffers(1, &this->_refractionDepthTexture);
}

GLuint RefractionFrameBuffer::getTexture() const
{
    return this->_refractionTexture;
}
