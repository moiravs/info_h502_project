#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <iostream>
#include <string>
#include <vector>

#include "renderableEntity.h"

class Cubemap : public RenderableEntity
{
    unsigned int textureID{};

public:
    explicit Cubemap(const std::vector<std::string>& faces, const std::shared_ptr<Renderer>& renderer);

    [[nodiscard]] unsigned int getTextureId() const;
};

#endif