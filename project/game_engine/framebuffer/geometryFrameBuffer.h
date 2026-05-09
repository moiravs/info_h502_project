
#ifndef INFOH502_CPP_GEOMETRYFRAMEBUFFER_H
#define INFOH502_CPP_GEOMETRYFRAMEBUFFER_H
#include "frameBuffer.h"

class GeometryFrameBuffer: public FrameBuffer {
    GLuint _colorTex;
    GLuint _depthTex;
    GLuint _normalTex;
public:
    GeometryFrameBuffer(int width, int height);

    void begin() override;

    void createTextures() override;
    void bindTextures() override;
    GLuint getColorTex() const;
    GLuint getDepthTex() const;
    GLuint getNormalTex() const;

    ~GeometryFrameBuffer() override;
};



#endif //INFOH502_CPP_GEOMETRYFRAMEBUFFER_H
