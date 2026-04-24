#ifndef WaterFrameBuffer_H
#define WaterFrameBuffer_H

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "manager/uboProvider.h"

class WaterFrameBuffer : public UboProvider
{
    GLuint reflectionFrameBuffer{};
    GLuint reflectionTexture{};
    GLuint reflectionDepthBuffer{};
    GLuint refractionFrameBuffer{};
    GLuint refractionTexture{};
    GLuint refractionDepthTexture{};

public:
    WaterFrameBuffer();
    ~WaterFrameBuffer() override;

    void bindReflectionFrameBuffer() const;

    void bindRefractionFrameBuffer() const;

    void setClipPlane(const glm::vec4 &plane) const;

    static void unbindCurrentFrameBuffer();

    [[nodiscard]] GLuint getReflectionTexture() const;

    [[nodiscard]] GLuint getRefractionTexture() const;

    [[nodiscard]] GLuint getRefractionDepthTexture() const;

    void initialiseReflectionFrameBuffer();

    void initialiseRefractionFrameBuffer();

    static void bindFrameBuffer(GLuint frameBuffer, int width, int height);

    static GLuint createFrameBuffer();

    static GLuint createTextureAttachment(int width, int height);

    static GLuint createDepthTextureAttachment(int width, int height);

    static GLuint createDepthBufferAttachment(int width, int height);
};

#endif
