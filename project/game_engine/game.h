#ifndef Game_H
#define Game_H

#include <vector>

#include "mesh/terrainMesh.h"
#include "entity/renderableEntity.h"

class Game
{

public:
    Game();
    static void renderScene(float delta, const std::vector<std::shared_ptr<Renderable>> &renderers, glm::mat4 lightSpaceMatrix, unsigned int shadowTex);
    static void checkTerrainCollision(const std::shared_ptr<Entity> &entity, const std::shared_ptr<TerrainMesh> &terrain);
};

#endif