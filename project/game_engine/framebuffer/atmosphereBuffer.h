
#ifndef INFOH502_CPP_AtmosphereFrameBuffer_H
#define INFOH502_CPP_AtmosphereFrameBuffer_H
#include "frameBuffer.h"

class AtmosphereFrameBuffer : public FrameBuffer
{
    GLuint _tex{};

public:
    AtmosphereFrameBuffer(int width, int height, GLuint depthTexture);
    void begin() override;
    void end() override;
    void createTextures() override;
    void bindTextures() override;
    [[nodiscard]] GLuint getTexture() const;
};

#endif // INFOH502_CPP_AtmosphereFrameBuffer_H
