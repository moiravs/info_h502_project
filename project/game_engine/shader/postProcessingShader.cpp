
#include "postProcessingShader.h"

PostProcessingShader::PostProcessingShader(const std::string& vertexPath, const std::string& fragmentPath)
: Shader(vertexPath, fragmentPath)
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
}

void PostProcessingShader::render() const
{
    glUseProgram(this->getID());

    this->linkTextures();

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

PostProcessingShader::~PostProcessingShader()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
