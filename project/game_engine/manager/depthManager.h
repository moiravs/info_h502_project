
#ifndef Shadow_H
#define Shadow_H

#include <iostream>
#include <memory>
#include <string>
#include "../shader.h"

class DepthManager
{
    DepthManager();
public:
    static std::shared_ptr<DepthManager> get();
    std::shared_ptr<Shader> shader;
    unsigned int depthMapFBO;
    unsigned int depthMap;
};

#endif