
#ifndef INFOH502_CPP_RENDERABLEENTITYMAKER_H
#define INFOH502_CPP_RENDERABLEENTITYMAKER_H

#include "renderableEntity.h"
#include "../culling/octree.h"

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
        OctreeManager::get()->put(obj, obj->getPosition());

        return obj;
    }
};



#endif //INFOH502_CPP_RENDERABLEENTITYMAKER_H
