#ifndef Game_H
#define Game_H

#include <vector>

#include "mesh/terrainMesh.h"
#include "renderer/waterRenderer.h"

class Game
{

public:
    Game();
    static void renderScene(float delta, const std::vector<std::shared_ptr<Renderable>> &renderers);
    static void checkTerrainCollision(const std::shared_ptr<Entity>& entity, const std::shared_ptr<TerrainMesh>& terrain);
};

#endif