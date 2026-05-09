
#ifndef INFOH502_CPP_FRAMEBUFFER_H
#define INFOH502_CPP_FRAMEBUFFER_H
#include "glad/glad.h"


class FrameBuffer {
protected:
    int _width;
    int _height;
    GLuint _fbo;

    void createDepthTexture(GLuint& tex) const;
    void createColorTexture(GLuint& tex, GLenum attachment) const;
    void createDepthBuffer(GLuint& buf) const;
public:
    virtual void createTextures()=0;
    virtual void bindTextures()=0;
    virtual ~FrameBuffer();
    FrameBuffer(int width, int height);
    virtual void begin();
    virtual void end();
};



#endif //INFOH502_CPP_FRAMEBUFFER_H
