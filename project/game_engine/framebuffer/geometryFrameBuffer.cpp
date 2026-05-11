
#include "geometryFrameBuffer.h"

#include "../../utils/utils.h"
#include "../../utils/constants.h"

GeometryFrameBuffer::GeometryFrameBuffer(const int width, const int height): FrameBuffer(width, height)
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
    createColorTexture(this->_materialTex, GL_COLOR_ATTACHMENT2);
    createDepthTexture(this->_depthTex);

    constexpr GLuint attachments[] =
    {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2
    };

    // this says that if the shader writes in location 0, it's mapped to
    // attachment 0 (here the color texture) and if in location 1, the normal tex
    // and 2 the material tex
    glDrawBuffers(3, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ERROR("Failed building the FBO.");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GeometryFrameBuffer::bindTextures()
{
    glActiveTexture(COLOR_TEX);
    glBindTexture(GL_TEXTURE_2D, this->_colorTex);

    glActiveTexture(NORMAL_TEX);
    glBindTexture(GL_TEXTURE_2D, this->_normalTex);

    glActiveTexture(DEPTH_TEX);
    glBindTexture(GL_TEXTURE_2D, this->_depthTex);

    glActiveTexture(MATERIAL_TEX);
    glBindTexture(GL_TEXTURE_2D, this->_materialTex);
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

GLuint GeometryFrameBuffer::getMaterialTex() const
{
    return this->_materialTex;
}

GeometryFrameBuffer::~GeometryFrameBuffer()
{
    glDeleteTextures(1, &this->_colorTex);
    glDeleteTextures(1, &this->_depthTex);
    glDeleteTextures(1, &this->_normalTex);
}
