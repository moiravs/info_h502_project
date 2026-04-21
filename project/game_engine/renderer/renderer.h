
#ifndef Renderer_H
#define Renderer_H
#include "../entity/object.h"

class Object;

class Renderer
{
protected:
    std::vector<std::shared_ptr<Object>> _objects;
public:
    virtual ~Renderer() = default;
    virtual void registerObject(std::shared_ptr<Object> object);
    void registerObjects(std::vector<std::shared_ptr<Object>>& objects);
    virtual void render() = 0;
};

#endif