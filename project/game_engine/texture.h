
#ifndef Texture_H
#define Texture_H

#include <string>
#include <iostream>

class Texture
{
    int m_Width{};
    int m_Height{};
    int m_Channels{};
    unsigned int texture0{};

public:

    Texture() = default;

    [[nodiscard]] int getWidth() const;

    explicit Texture(const std::string &filePath);

    ~Texture();

    void bind() const;

    [[nodiscard]] int getTexture() const;

    static void unbind();
};

#endif