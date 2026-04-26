
#ifndef INFOH502_CPP_RENDERABLE_H
#define INFOH502_CPP_RENDERABLE_H

class Renderable
{
public:
    virtual ~Renderable() = default;
    virtual void render(float delta)=0;
};

#endif //INFOH502_CPP_RENDERABLE_H
