
#ifndef INFOH502_CPP_REFLECTIONFRAMEBUFFER_H
#define INFOH502_CPP_REFLECTIONFRAMEBUFFER_H
#include "../../utils/constants.h"
#include "../manager/uboProvider.h"
#include "frameBuffer.h"

class ReflectionFrameBuffer: public FrameBuffer, public UboProvider {
    GLuint _reflectionTexture{};
    GLuint _reflectionDepthBuffer{};
public:
    explicit ReflectionFrameBuffer(int width=REFLECTION_WIDTH, int height=REFLECTION_HEIGHT);
    void createTextures() override;
    void bindTextures() override;
    void setClipPlane(const glm::vec4 &plane) const;
    void begin() override;
    void end() override;
    GLuint getTexture() const;
    ~ReflectionFrameBuffer() override;
};

#endif //INFOH502_CPP_REFLECTIONFRAMEBUFFER_H
