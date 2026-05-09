#include "texture.h"

#include <stb_image.h>
#include <glad/glad.h>
#include "../utils/utils.h"

int Texture::getWidth() const
{
    return m_Width;
}

Texture::Texture(const std::string &filePath) : m_Width(0), m_Height(0), m_Channels(0)
{
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    float amount = 0.0f;
    if (GLAD_GL_EXT_texture_filter_anisotropic)
    { // Check if extension exists
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &amount);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);
    }
    int width, height, nrChannels;

    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
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
        std::string errormsg = "Unable to load texture from file";
        ERROR(errormsg + filePath.c_str());
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &texture0);
}

void Texture::bind() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);
}

int Texture::getTexture() const
{
    return texture0;
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
