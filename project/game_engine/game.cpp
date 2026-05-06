#include "game.h"
#include "renderer/meshRenderer.h"
#include "prop/prop.h"
#include "renderer/instancedRenderer.h"

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

void Game::renderShadowsInstanced(const float delta, const std::vector<std::shared_ptr<Renderable>> &renderers, glm::mat4 lightSpaceMatrix, unsigned int shadowTex, const std::shared_ptr<Shader> shadowShader)
{
    shadowShader->use();
    shadowShader->setMatrix4("lightSpaceMatrix", lightSpaceMatrix);
    for (const auto &i : renderers)
    {

        if (auto mesh = std::dynamic_pointer_cast<RenderableEntity>(i))
        {

            if (auto meshRef = std::dynamic_pointer_cast<InstancedRenderer>(mesh->getRenderer()))
            {
                meshRef->setShadowData(shadowTex, lightSpaceMatrix);
                meshRef->updateUniforms();
                meshRef->renderShadows(shadowShader);
            }
        }
        if (auto mesh = std::dynamic_pointer_cast<Prop>(i))
        {
            if (auto meshRef = std::dynamic_pointer_cast<InstancedRenderer>(mesh->getMainObject()->getRenderer()))
            {
                meshRef->setShadowData(shadowTex, lightSpaceMatrix);
                meshRef->updateUniforms();
                meshRef->renderShadows(shadowShader);
            }
        }
        else
        {
            i->render(delta);
        }
    }
}

void Game::renderShadows(const float delta, const std::vector<std::shared_ptr<Renderable>> &renderers, glm::mat4 lightSpaceMatrix, unsigned int shadowTex, const std::shared_ptr<Shader> shadowShader)
{
    shadowShader->use();
    shadowShader->setMatrix4("lightSpaceMatrix", lightSpaceMatrix);
    for (const auto &i : renderers)
    {

        if (auto mesh = std::dynamic_pointer_cast<RenderableEntity>(i))
        {

            if (auto meshRef = std::dynamic_pointer_cast<MeshRenderer>(mesh->getRenderer()))
            {
                meshRef->setShadowData(shadowTex, lightSpaceMatrix);
                meshRef->updateUniforms();
                meshRef->renderShadows(shadowShader);
            }
        }
        if (auto mesh = std::dynamic_pointer_cast<Prop>(i))
        {
            if (auto meshRef = std::dynamic_pointer_cast<MeshRenderer>(mesh->getMainObject()->getRenderer()))
            {
                meshRef->setShadowData(shadowTex, lightSpaceMatrix);
                meshRef->updateUniforms();
                meshRef->renderShadows(shadowShader);
            }
        }
        else
        {
            i->render(delta);
        }
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
