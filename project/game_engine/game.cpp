#include "game.h"
#include "renderer/meshRenderer.h"
#include "prop/prop.h"

Game::Game()
{
}
void Game::renderScene(const float delta, const std::vector<std::shared_ptr<Renderable>> &renderers, glm::mat4 lightSpaceMatrix, unsigned int shadowTex)
{
    for (const auto &i : renderers)
    {
        std::cout << "render" << typeid(*i).name() << std::endl;
        if (auto mesh = std::dynamic_pointer_cast<RenderableEntity>(i))
        {
            std::cout << "here" << std::endl;
            if (auto meshRef = std::dynamic_pointer_cast<MeshRenderer>(mesh->getRenderer()))
            {
                std::cout << "next" << std::endl;
                meshRef->setShadowData(shadowTex, lightSpaceMatrix);
                meshRef->updateUniforms();
            }
        }

        if (auto mesh = std::dynamic_pointer_cast<Prop>(i))
        {
            std::cout << "hereprop" << std::endl;
            if (auto meshRef = std::dynamic_pointer_cast<MeshRenderer>(mesh->getMainObject()->getRenderer()))
            {
                std::cout << "next" << std::endl;
                meshRef->setShadowData(shadowTex, lightSpaceMatrix);
                meshRef->updateUniforms();
            }
        }
        i->render(delta);
    }
}

void Game::checkTerrainCollision(const std::shared_ptr<Entity> &entity, const std::shared_ptr<TerrainMesh> &terrain)
{
    glm::vec3 pos = entity->getPosition();

    // Get the height of the floor at the entity's current X, Z
    float terrainHeight = terrain->getHeight(pos.x, pos.z);

    // If the entity is below the ground, snap it back to the surface
    if (pos.y < terrainHeight)
    {
        entity->setPosition(pos.x, terrainHeight, pos.z);
    }
}
