
#include "geometryFrameBuffer.h"

#include "../../utils/utils.h"

GeometryFrameBuffer::GeometryFrameBuffer(const int width, const int height) :
    FrameBuffer(width, height), _colorTex(0), _depthTex(0), _normalTex(0)
{
}

void GeometryFrameBuffer::begin()
{
    this->FrameBuffer::begin();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GeometryFrameBuffer::createTextures()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
    createColorTexture(this->_colorTex, GL_COLOR_ATTACHMENT0);
    createColorTexture(this->_normalTex, GL_COLOR_ATTACHMENT1);
    createDepthTexture(this->_depthTex);

    constexpr GLuint attachments[] =
    {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1
    };

    // this says that if the shader writes in location 0, it's mapped to
    // attachment 0 (here the color texture) and if in location 1, the normal tex.
    glDrawBuffers(2, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ERROR("Failed building the FBO.");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GeometryFrameBuffer::bindTextures()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->_colorTex);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->_normalTex);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, this->_depthTex);
}

GLuint GeometryFrameBuffer::getColorTex() const
{
    return this->_colorTex;
}

GLuint GeometryFrameBuffer::getDepthTex() const
{
    return this->_depthTex;
}

GLuint GeometryFrameBuffer::getNormalTex() const
{
    return this->_normalTex;
}

GeometryFrameBuffer::~GeometryFrameBuffer()
{
    glDeleteTextures(1, &this->_colorTex);
    glDeleteTextures(1, &this->_depthTex);
    glDeleteTextures(1, &this->_normalTex);
}
