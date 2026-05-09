
#ifndef INFOH502_CPP_RENDERABLEENTITYMAKER_H
#define INFOH502_CPP_RENDERABLEENTITYMAKER_H

#include "../mesh/heightMap.h"
#include "object.h"

class Prop;
class RenderableEntityMaker {
    RenderableEntityMaker()=default;

    static glm::vec3 calculateNormal(int x, int z, int imageWidth, int imageHeight, const std::vector<float>& vertices);
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

    static std::pair<std::shared_ptr<HeightMap>, std::shared_ptr<Prop>> terrainFromTexture(
        const std::string& texturePath, float width, float depth);
};



#endif //INFOH502_CPP_RENDERABLEENTITYMAKER_H
