#ifndef FIREPARTICLE_H
#define FIREPARTICLE_H

#include <glad/glad.h>
#include <glm/gtx/color_space.hpp>

#include <string>
#include <fstream>
#include <memory>
#include "shader.h"
#include "texture.h"
#include "entity/object.h"
#include "mainCamera.h"
#include "particleGenerator.h"

class FireGenerator : public ParticleGenerator
{
    int _height;

public:
    void update(double delta, double currentTime) override;
    FireGenerator(const std::shared_ptr<Shader> shader, int height);
};

#endif
