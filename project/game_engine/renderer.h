
#ifndef Renderer_H
#define Renderer_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <vector>

class Renderer
{
public:
    virtual void render() = 0;
};

#endif