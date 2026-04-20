#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <iostream>
#include <string>
#include <vector>

class Cubemap
{
    unsigned int textureID{};

public:
    explicit Cubemap(const std::vector<std::string>& faces);

    [[nodiscard]] unsigned int getTextureId() const;
};

#endif