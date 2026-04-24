#include "shader.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "manager/uboManager.h"
#include "../utils/constants.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }

    GLuint vertex = compileShader(vertexCode, GL_VERTEX_SHADER);
    GLuint fragment = compileShader(fragmentCode, GL_FRAGMENT_SHADER);
    this->ID = compileProgram(vertex, fragment);

    this->setMatrix4("model", this->model);

    UboManager::get().registerShader(this);
}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::setInteger(const GLchar* name, const GLint value) const
{
    this->use();
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setFloat(const GLchar* name, const GLfloat value) const
{
    this->use();

    glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::setVector2f(const GLchar* name, const GLfloat x, const GLfloat y) const
{
    this->use();

    glUniform2f(glGetUniformLocation(ID, name), x, y);
}

void Shader::setVector3f(const GLchar* name, const GLfloat x, const GLfloat y, const GLfloat z) const
{
    this->use();

    glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}

void Shader::setVector3f(const GLchar* name, const glm::vec3& value) const
{
    this->use();

    glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
}

void Shader::setVector4f(const GLchar* name, const glm::vec4& value) const
{
    this->use();
    glUniform4f(glGetUniformLocation(ID, name), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const GLchar* name, const glm::mat4& matrix) const
{
    this->use();
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

GLuint Shader::getID() const
{
    return this->ID;
}

GLuint Shader::compileShader(const std::string& shaderCode, const GLenum shaderType)
{
    const GLuint shader = glCreateShader(shaderType);
    const char *code = shaderCode.c_str();
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    GLchar infoLog[1024];
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::string t = "undetermined";
        if (shaderType == GL_VERTEX_SHADER)
        {
            t = "vertex shader";
        }
        else if (shaderType == GL_FRAGMENT_SHADER)
        {
            t = "fragment shader";
        }
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of the " << t << ": " << shaderType << infoLog << std::endl;
    }
    return shader;
}

GLuint Shader::compileProgram(const GLuint vertexShader, const GLuint fragmentShader)
{
    const GLuint programID = glCreateProgram();

    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    GLchar infoLog[1024];
    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programID, 1024, nullptr, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR:  " << infoLog << std::endl;
    }
    return programID;
}
