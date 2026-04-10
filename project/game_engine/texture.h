
#ifndef Texture_H
#define Texture_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "terrainGeneration.h"
#include <vector>
#include "../utils/utils.h"

#include <stb_image.h>

class Texture
{
public:
    int m_Width;
    int m_Height;
    int m_Channels;
    unsigned int texture0;

    Texture() = default;

    int getWidth()
    {
        return m_Width;
    }

    Texture(const std::string &filePath)
    {
        glGenTextures(1, &texture0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);

        // std::cout << filePath.c_str() << std::endl;
        unsigned char *data = stbi_load(PATH_TO_SRC "/../assets/textures/wall.jpg", &width, &height, &nrChannels, 0);
        // unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = GL_RED;
            if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }
        else
        {
            ERROR("Unable to load texture from file");
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