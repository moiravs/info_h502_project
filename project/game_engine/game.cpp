#include "game.h"

#include "depth/depthMap.h"
#include "prop/prop.h"
#include "renderer/meshRenderer.h"

#include "mesh/heightMap.h"

Game::Game()
{
}

void Game::renderScene(const float delta, const std::vector<std::shared_ptr<Renderable>> &renderers)
{
    for (const auto &i : renderers)
    {
        i->render(delta);
    }
}

void Game::renderShadows(const std::vector<std::shared_ptr<Renderable>> &renderables, const std::shared_ptr<DepthMap> &depthMap)
{
    depthMap->prepare();

    for (const auto &i : renderables)
    {
        i->renderDepth(depthMap);
    }

    depthMap->cleanup();
}

void Game::checkTerrainCollision(const std::shared_ptr<Entity> &entity, const std::shared_ptr<HeightMap> &heighMap)
{
    glm::vec3 pos = entity->getPosition();

    // Get the height of the floor at the entity's current X, Z
    float terrainHeight = heighMap->getHeight(pos.x, pos.z);

    // If the entity is below the ground, snap it back to the surface
    if (pos.y < terrainHeight)
    {
        entity->setPosition(pos.x, terrainHeight, pos.z);
    }
}
