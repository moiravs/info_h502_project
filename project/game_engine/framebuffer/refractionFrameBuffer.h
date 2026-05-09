
#ifndef INFOH502_CPP_REFRACTIONFRAMEBUFFER_H
#define INFOH502_CPP_REFRACTIONFRAMEBUFFER_H
#include "frameBuffer.h"

#include "../../utils/constants.h"

class RefractionFrameBuffer: public FrameBuffer {
    GLuint _refractionTexture{};
    GLuint _refractionDepthTexture{};
public:
    explicit RefractionFrameBuffer(int width=REFRACTION_WIDTH, int height=REFRACTION_HEIGHT);

    void createTextures() override;
    void bindTextures() override;
    void begin() override;
    void end() override;
    ~RefractionFrameBuffer() override;
    GLuint getTexture() const;
};



#endif //INFOH502_CPP_REFRACTIONFRAMEBUFFER_H
