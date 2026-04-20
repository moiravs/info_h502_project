#ifndef Skybox_H
#define Skybox_H

#include <string>
#include <sstream>
#include <vector>

#include "cubemap.h"

class Skybox : public Cubemap
{
    unsigned int textureID;
    std::vector<float> skyboxVertices;

public:
    explicit Skybox(const std::vector<std::string>& faces);

    [[nodiscard]] const std::vector<float>& getVertices() const;
};

#endif