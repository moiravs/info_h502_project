
#ifndef Texture_H
#define Texture_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "terrainGeneration.h"
#include <vector>

#include <stb_image.h>

class Texture
{
public:
    int m_Width;
    int m_Height;
    int m_Channels;
    unsigned int texture0;
    unsigned char *data;

    Texture() = default;

    int getWidth()
    {
        return m_Width;
    }

    unsigned char *getData()
    {
        return data;
    }

    int getChannels()
    {
        return m_Channels;
    }

    int getHeight()
    {
        return m_Height;
    }

    Texture(const std::string &filePath)
    {

        glGenTextures(1, &texture0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_set_flip_vertically_on_load(true);

        data = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_Channels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

    }

    ~Texture()
    {
        glDeleteTextures(1, &texture0);
    }

    void bind() const
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
    }

    int getTexture()
    {
        return texture0;
    }

    void unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

#endif