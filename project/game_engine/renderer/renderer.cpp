#include "renderer.h"

void Renderer::registerObject(std::shared_ptr<Object> object)
{
    this->_objects.push_back(object);
}

void Renderer::registerObjects(std::vector<std::shared_ptr<Object>>& objects)
{
    for (auto& object: objects)
        this->registerObject(object);
}
