#include "waterFrameBuffer.h"
#include <memory>
#include "../utils/constants.h"
#include <glm/gtc/type_ptr.hpp>

WaterFrameBuffer::WaterFrameBuffer()
{
    initialiseReflectionFrameBuffer();
    initialiseRefractionFrameBuffer();

    glGenBuffers(1, &uboWater);
    glBindBuffer(GL_UNIFORM_BUFFER, uboWater);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, uboWater);
}

void WaterFrameBuffer::cleanUp() const
{
    // call when closing the game
    glDeleteFramebuffers(1, &reflectionFrameBuffer);
    glDeleteTextures(1, &reflectionTexture);
    glDeleteRenderbuffers(1, &reflectionDepthBuffer);
    glDeleteFramebuffers(1, &refractionFrameBuffer);
    glDeleteTextures(1, &refractionTexture);
    glDeleteTextures(1, &refractionDepthTexture);
}

void WaterFrameBuffer::bindReflectionFrameBuffer() const
{
    // call before rendering to this FBO
    bindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void WaterFrameBuffer::bindRefractionFrameBuffer() const
{
    // call before rendering to this FBO
    bindFrameBuffer(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void WaterFrameBuffer::setClipPlane(const glm::vec4 &plane) const
{
    glBindBuffer(GL_UNIFORM_BUFFER, uboWater);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(plane));
}

void WaterFrameBuffer::connectShader(const std::shared_ptr<Shader> &shader)
{
    const GLuint blockIndex = glGetUniformBlockIndex(shader->getID(), "WaterData");
    if (blockIndex != GL_INVALID_INDEX)
    {
        glUniformBlockBinding(shader->getID(), blockIndex, 3);
    }
}

void WaterFrameBuffer::unbindCurrentFrameBuffer()
{
    // call to switch to default frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

int WaterFrameBuffer::getReflectionTexture() const
{
    // get the resulting texture
    return reflectionTexture;
}

int WaterFrameBuffer::getRefractionTexture() const
{
    // get the resulting texture
    return refractionTexture;
}

int WaterFrameBuffer::getRefractionDepthTexture() const
{
    // get the resulting depth texture
    return refractionDepthTexture;
}

void WaterFrameBuffer::initialiseReflectionFrameBuffer()
{
    reflectionFrameBuffer = createFrameBuffer();
    reflectionTexture = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
    reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
    unbindCurrentFrameBuffer();
}

void WaterFrameBuffer::initialiseRefractionFrameBuffer()
{
    refractionFrameBuffer = createFrameBuffer();
    refractionTexture = createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
    refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
    unbindCurrentFrameBuffer();
}

void WaterFrameBuffer::bindFrameBuffer(const int frameBuffer, const int width, const int height)
{
    glBindTexture(GL_TEXTURE_2D, 0); // To make sure the texture isn't bound
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, width, height);
}

int WaterFrameBuffer::createFrameBuffer()
{
    GLuint frameBuffer;
    glGenFramebuffers(1, &frameBuffer);
    // generate name for frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    // create the framebuffer
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    // indicate that we will always render to color attachment 0
    return frameBuffer;
}

int WaterFrameBuffer::createTextureAttachment(const int width, const int height)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                         texture, 0);
    return texture;
}

int WaterFrameBuffer::createDepthTextureAttachment(const int width, const int height)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                         texture, 0);
    return texture;
}

int WaterFrameBuffer::createDepthBufferAttachment(const int width, const int height)
{
    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width,
                          height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthBuffer);
    return depthBuffer;
}
