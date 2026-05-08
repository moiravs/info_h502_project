
#ifndef INFOH502_CPP_RENDERABLE_H
#define INFOH502_CPP_RENDERABLE_H

#include <memory>

class DepthMap;
class Renderable
{
public:
    virtual ~Renderable() = default;
    virtual void render(float delta)=0;
    virtual void renderDepth(const std::shared_ptr<DepthMap> &depthMap)=0;
};

#endif //INFOH502_CPP_RENDERABLE_H
