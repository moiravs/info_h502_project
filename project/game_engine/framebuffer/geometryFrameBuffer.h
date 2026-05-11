
#ifndef INFOH502_CPP_GEOMETRYFRAMEBUFFER_H
#define INFOH502_CPP_GEOMETRYFRAMEBUFFER_H
#include "frameBuffer.h"

class GeometryFrameBuffer: public FrameBuffer {
    GLuint _colorTex {};
    GLuint _depthTex {};
    GLuint _normalTex {};
    GLuint _materialTex {};
public:
    GeometryFrameBuffer(int width, int height);

    void begin() override;

    void createTextures() override;
    void bindTextures() override;
    [[nodiscard]] GLuint getColorTex() const;
    [[nodiscard]] GLuint getDepthTex() const;
    [[nodiscard]] GLuint getNormalTex() const;
    [[nodiscard]] GLuint getMaterialTex() const;

    ~GeometryFrameBuffer() override;
};



#endif //INFOH502_CPP_GEOMETRYFRAMEBUFFER_H
