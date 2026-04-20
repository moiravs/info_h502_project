
#ifndef Renderer_H
#define Renderer_H

class Renderer
{
public:
    virtual ~Renderer() = default;
    virtual void render() = 0;
};

#endif