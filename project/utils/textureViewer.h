
#ifndef INFOH502_CPP_TEXTUREVIEWER_H
#define INFOH502_CPP_TEXTUREVIEWER_H

#include <glad/glad.h>
#include <memory>


class Shader;
class TextureViewer
{
    GLuint vao = 0;
    GLuint vbo = 0;

    std::shared_ptr<Shader> textureShader=nullptr;
public:
    TextureViewer();
    ~TextureViewer();

    void render(GLuint textureID) const;
};

#endif //INFOH502_CPP_TEXTUREVIEWER_H
