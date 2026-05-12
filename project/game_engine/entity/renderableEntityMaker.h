
#ifndef INFOH502_CPP_RENDERABLEENTITYMAKER_H
#define INFOH502_CPP_RENDERABLEENTITYMAKER_H

#include "../mesh/heightMap.h"
#include "object.h"

class Prop;
class RenderableEntityMaker {
    RenderableEntityMaker()=default;
public:
    template <typename T, typename Renderer, typename RendererArg, typename... EntityArgs>
    static std::shared_ptr<T> makeRenderable(const RendererArg& rendererArg, EntityArgs&&... entityArgs)
    {
        auto renderer = std::make_shared<Renderer>(rendererArg);

        auto obj = std::make_shared<T>(
            std::forward<EntityArgs>(entityArgs)...,
            renderer
        );

        renderer->registerEntity(obj);

        if (const std::shared_ptr<Object> o = std::dynamic_pointer_cast<Object>(obj))
            o->updateBounds();

        return obj;
    }

    template <typename T, typename Renderer, typename... EntityArgs>
    static std::shared_ptr<T> makeRenderable(EntityArgs&&... entityArgs)
    {
        auto renderer = std::make_shared<Renderer>();

        auto obj = std::make_shared<T>(
            std::forward<EntityArgs>(entityArgs)...,
            renderer
        );

        renderer->registerEntity(obj);

        if (const std::shared_ptr<Object> o = std::dynamic_pointer_cast<Object>(obj))
            o->updateBounds();

        return obj;
    }
};



#endif //INFOH502_CPP_RENDERABLEENTITYMAKER_H
