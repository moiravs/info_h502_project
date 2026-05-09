
#include "textureViewer.h"

#include "../game_engine/shader.h"

TextureViewer::TextureViewer()
{
    constexpr float quad[] =
    {
        // pos      // uv
        -1.f,  1.f, 0.f, 1.f,
        -1.f, -1.f, 0.f, 0.f,
         1.f, -1.f, 1.f, 0.f,

        -1.f,  1.f, 0.f, 1.f,
         1.f, -1.f, 1.f, 0.f,
         1.f,  1.f, 1.f, 1.f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)(2 * sizeof(float))
    );

    textureShader = std::make_shared<Shader>(
        PATH_TO_SRC "/../assets/shaders/debugtexture.vert",
        PATH_TO_SRC "/../assets/shaders/debugtexture.frag");
}

TextureViewer::~TextureViewer()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void TextureViewer::render(const GLuint textureID) const
{
    glUseProgram(textureShader->getID());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glUniform1i(
        glGetUniformLocation(textureShader->getID(), "tex"),
        0
    );

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
