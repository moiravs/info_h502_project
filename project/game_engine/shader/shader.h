#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include "../entity/light/light.h"

class Shader
{
    GLuint ID;

public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    virtual ~Shader();
    void use() const;

    [[nodiscard]] GLuint getID() const;

    void setInteger(const GLchar *name, GLint value) const;

    void setFloat(const GLchar *name, GLfloat value) const;

    void setVector2f(const GLchar *name, GLfloat x, GLfloat y) const;
    void setVector2f(const GLchar *name, const glm::vec2& vec2) const;

    void setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z) const;

    void setVector3f(const GLchar *name, const glm::vec3 &value) const;

    void setVector4f(const GLchar *name, const glm::vec4 &value) const;

    void setMatrix4(const GLchar *name, const glm::mat4 &matrix) const;

private:
    static GLuint compileShader(const std::string &shaderCode, GLenum shaderType);

    static GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader);
};
#endif