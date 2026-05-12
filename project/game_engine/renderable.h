
#ifndef INFOH502_CPP_RENDERABLE_H
#define INFOH502_CPP_RENDERABLE_H

#include <memory>

class Shader;
class Renderable
{
public:
    virtual ~Renderable() = default;
    virtual void render()=0;
    virtual void update(float delta)=0;
    virtual void renderWithShader(const std::shared_ptr<Shader> &shader)=0;
};

#endif //INFOH502_CPP_RENDERABLE_H
