#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>

#include "entity/camera.h"
#include "entity/light.h"

class Shader
{
    GLuint ID;
    glm::mat4 model = glm::mat4(1.0);

public:

    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    void use() const;

    [[nodiscard]] GLuint getID() const;

    void setLight(const std::shared_ptr<Light>& light) const;

    void updatePos(const std::shared_ptr<Camera>& currentCamera) const;

    void setInteger(const GLchar *name, GLint value) const;

    void setFloat(const GLchar *name, GLfloat value) const;

    void setVector2f(const GLchar *name, GLfloat x, GLfloat y) const;

    void setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z) const;

    void setVector3f(const GLchar *name, const glm::vec3 &value) const;

    void setVector4f(const GLchar *name, const glm::vec4 &value) const;

    void setMatrix4(const GLchar *name, const glm::mat4 &matrix) const;

private:
    static GLuint compileShader(const std::string& shaderCode, GLenum shaderType);

    static GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader);
};
#endif