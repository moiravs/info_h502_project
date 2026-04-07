#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "camera.h"
#include "displaymanager.h"
class Shader
{
public:
    GLuint ID;
    glm::mat4 model = glm::mat4(1.0);

    Shader(const char *vertexPath, const char *fragmentPath, const char *tessControlPath = nullptr, const char *tessEvalPath = nullptr)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        std::string tessControlCode;
        std::string tessEvalCode;
        std::ifstream tcShaderFile;
        std::ifstream teShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        tcShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        teShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {

            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            // In your shader loading code, add debug output:

            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

            if (tessControlPath != nullptr)
            {
                tcShaderFile.open(tessControlPath);
                std::stringstream tcShaderStream;
                tcShaderStream << tcShaderFile.rdbuf();
                tcShaderFile.close();
                tessControlCode = tcShaderStream.str();
            }
            if (tessEvalPath != nullptr)
            {
                teShaderFile.open(tessEvalPath);
                std::stringstream teShaderStream;
                teShaderStream << teShaderFile.rdbuf();
                teShaderFile.close();
                tessEvalCode = teShaderStream.str();
            }
        }
        catch (std::ifstream::failure &e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();

        GLuint vertex = compileShader(vertexCode, GL_VERTEX_SHADER);
        GLuint fragment = compileShader(fragmentCode, GL_FRAGMENT_SHADER);

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (tessControlPath != nullptr)
        {
            GLuint tessControl = compileShader(tessControlCode, GL_TESS_CONTROL_SHADER);
            glAttachShader(ID, tessControl);
        }
        if (tessEvalPath != nullptr)
        {
            GLuint tessEval = compileShader(tessEvalCode, GL_TESS_EVALUATION_SHADER);
            glAttachShader(ID, tessEval);
        }

        glLinkProgram(ID);
        this->setMatrix4("model", model);
    }

    Shader(std::string vShaderCode, std::string fShaderCode)
    {
        GLuint vertex = compileShader(vShaderCode, GL_VERTEX_SHADER);
        GLuint fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER);
        ID = compileProgram(vertex, fragment);
    }

    void use()
    {
        glUseProgram(ID);
    }

    void updatePos(Camera currentCamera)
    {
        glm::mat4 projection = currentCamera.GetProjectionMatrix(glm::radians(currentCamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.0f);
        glm::mat4 view = currentCamera.GetViewMatrix();

        this->setMatrix4("projection", projection);
        this->setMatrix4("view", view);
    }
    void setInteger(const GLchar *name, GLint value)
    {
        this->use();
        glUniform1i(glGetUniformLocation(ID, name), value);
    }
    void setFloat(const GLchar *name, GLfloat value)
    {
        this->use();

        glUniform1f(glGetUniformLocation(ID, name), value);
    }
    void setVector2f(const GLchar *name, GLfloat x, GLfloat y)
    {
        this->use();

        glUniform2f(glGetUniformLocation(ID, name), x, y);
    }
    void setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z)
    {
        this->use();

        glUniform3f(glGetUniformLocation(ID, name), x, y, z);
    }
    void setVector3f(const GLchar *name, const glm::vec3 &value)
    {
        this->use();

        glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
    }
    void setVector4f(const GLchar *name, const glm::vec4 &value)
    {
        this->use();
        glUniform4f(glGetUniformLocation(ID, name), value.x, value.y, value.z, value.w);
    }
    void setMatrix4(const GLchar *name, const glm::mat4 &matrix)
    {
        this->use();
        glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

private:
    GLuint compileShader(std::string shaderCode, GLenum shaderType)
    {
        GLuint shader = glCreateShader(shaderType);
        const char *code = shaderCode.c_str();
        glShaderSource(shader, 1, &code, NULL);
        glCompileShader(shader);

        GLchar infoLog[1024];
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
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

    GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader)
    {
        GLuint programID = glCreateProgram();

        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);

        GLchar infoLog[1024];
        GLint success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programID, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR:  " << infoLog << std::endl;
        }
        return programID;
    }
};
#endif