#ifndef Game_H
#define Game_H

#include <vector>

#include "entity/renderableEntity.h"
#include "shader.h"


class DepthMap;
class HeightMap;

class Game
{

public:
    Game();
    static void renderScene(float delta, const std::vector<std::shared_ptr<Renderable>> &renderers);
    static void checkTerrainCollision(const std::shared_ptr<Entity> &entity, const std::shared_ptr<HeightMap> &heightMap);
    static void renderShadows(const std::vector<std::shared_ptr<Renderable>> &renderables, const std::shared_ptr<DepthMap> &depthMap);
};

#endif