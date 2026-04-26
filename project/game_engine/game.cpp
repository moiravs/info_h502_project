#include "game.h"

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

void Game::checkTerrainCollision(const std::shared_ptr<Entity>& entity, const TerrainGeneration& terrain)
{
    glm::vec3 pos = entity->getPosition();

    // Get the height of the floor at the entity's current X, Z
    float terrainHeight = terrain.getHeight(pos.x, pos.z);

    // If the entity is below the ground, snap it back to the surface
    if (pos.y < terrainHeight)
    {
        entity->setPosition(pos.x, terrainHeight, pos.z);
    }
}
