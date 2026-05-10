
#ifndef INFOH502_CPP_LIGHTFRAMEBUFFER_H
#define INFOH502_CPP_LIGHTFRAMEBUFFER_H
#include "frameBuffer.h"

class LightFrameBuffer: public FrameBuffer {
    GLuint _tex {};
public:
    LightFrameBuffer(int width, int height);
    void begin() override;
    void end() override;
    void createTextures() override;
    void bindTextures() override;
    GLuint getTexture() const;
};



#endif //INFOH502_CPP_LIGHTFRAMEBUFFER_H
