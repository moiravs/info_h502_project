
#include "reflectionFrameBuffer.h"
#include "../../utils/utils.h"
#include "../manager/mainCamera.h"
#include "glm/gtc/type_ptr.hpp"

ReflectionFrameBuffer::ReflectionFrameBuffer(const int width, const int height) :
    FrameBuffer(width, height), UboProvider("WaterData", sizeof(glm::vec4))
{}

void ReflectionFrameBuffer::createTextures()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
    createColorTexture(this->_reflectionTexture, GL_COLOR_ATTACHMENT0);
    createDepthBuffer(this->_reflectionDepthBuffer);

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

void ReflectionFrameBuffer::bindTextures()
{
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, this->_reflectionTexture);
}

void ReflectionFrameBuffer::begin()
{
    FrameBuffer::begin();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CLIP_DISTANCE0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    MainCamera::get()->prepareReflection(WATER_HEIGHT);
    MainCamera::get()->updateUBO();
}

void ReflectionFrameBuffer::end()
{
    FrameBuffer::end();
    glDisable(GL_CLIP_DISTANCE0);
    MainCamera::get()->resetCameraAfterReflection(WATER_HEIGHT);
    MainCamera::get()->updateUBO();
}

ReflectionFrameBuffer::~ReflectionFrameBuffer()
{
    glDeleteTextures(1, &this->_reflectionTexture);
    glDeleteRenderbuffers(1, &this->_reflectionDepthBuffer);
}

void ReflectionFrameBuffer::setClipPlane(const glm::vec4& plane) const
{
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0,
                     sizeof(glm::vec4),
                     glm::value_ptr(plane));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLuint ReflectionFrameBuffer::getTexture() const
{
    return this->_reflectionTexture;
}
