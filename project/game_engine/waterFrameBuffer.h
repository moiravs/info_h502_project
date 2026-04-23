#ifndef WaterFrameBuffer_H
#define WaterFrameBuffer_H

#include <glad/glad.h>

#include <glm/glm.hpp>

class WaterFrameBuffer
{
    GLuint reflectionFrameBuffer{};
    GLuint reflectionTexture{};
    GLuint reflectionDepthBuffer{};
    GLuint refractionFrameBuffer{};
    GLuint refractionTexture{};
    GLuint refractionDepthTexture{};

    GLuint uboWater{};

public:
    WaterFrameBuffer();
    ~WaterFrameBuffer();

    void bindReflectionFrameBuffer() const;

    void bindRefractionFrameBuffer() const;

    void setClipPlane(const glm::vec4 &plane) const;

    static void unbindCurrentFrameBuffer();

    [[nodiscard]] int getReflectionTexture() const;

    [[nodiscard]] int getRefractionTexture() const;

    [[nodiscard]] int getRefractionDepthTexture() const;

    void initialiseReflectionFrameBuffer();

    void initialiseRefractionFrameBuffer();

    static void bindFrameBuffer(int frameBuffer, int width, int height);

    static int createFrameBuffer();

    static int createTextureAttachment(int width, int height);

    static int createDepthTextureAttachment(int width, int height);

    static int createDepthBufferAttachment(int width, int height);
};

#endif
