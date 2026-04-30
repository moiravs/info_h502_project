
#ifndef Shadow_H
#define Shadow_H

#include <string>
#include <iostream>
#include <memory>
#include "shader.h"

class Shadow
{

public:
    Shadow();
    std::shared_ptr<Shader> shadowShader;
    unsigned int depthMapFBO;
    unsigned int depthMap;
};

#endif