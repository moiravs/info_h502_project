#include "game.h"

#include "depth/depthMapFrameBuffer.h"
#include "prop/prop.h"
#include "renderer/meshRenderer.h"

#include "mesh/heightMap.h"
#include "entity/text.h"
#include <map>

Game::Game()
{
    success.openFromFile(PATH_TO_SRC "/../assets/music/success.mp3");
}

void Game::checkIfPlaneInRing(const std::shared_ptr<Prop> plane, const std::shared_ptr<Prop> rings, const std::shared_ptr<HeightMap> &heightMap)
{
    auto planeObj = plane->getMainObject();
    glm::vec3 planePos = planeObj->getPosition();

    std::shared_ptr<Renderable> toRemove = nullptr;

    constexpr int maxRandom = PLAN_SIZE_X / 2;
    constexpr int minRandom = -PLAN_SIZE_X / 2;

    constexpr int maxHeightRandom = 10;
    constexpr int minHeightRandom = 50;

    for (auto &renderable : rings->getRenderables())
    {
        auto ring = std::dynamic_pointer_cast<Object>(renderable);
        if (!ring)
            continue;

        glm::vec3 ringCenter = ring->getCenter();
        float ringRadius = ring->getRadius();
        float distToCenter = glm::distance(planePos, ringCenter);
        glm::vec3 ringNormal = glm::normalize(glm::vec3(ring->getModel()[1]));
        float distToFlatSurface = glm::abs(glm::dot(planePos - ringCenter, ringNormal));

        if (distToCenter < ringRadius && distToFlatSurface < 2.0f)
        {
            numberOfRings += 1;
            success.play();

            const float x = minRandom + static_cast<float>(rand()) / RAND_MAX * (maxRandom - minRandom);
            const float z = minRandom + static_cast<float>(rand()) / RAND_MAX * (maxRandom - minRandom);
            const float y = glm::max(minHeightRandom + static_cast<float>(rand()) / RAND_MAX * (maxHeightRandom - minHeightRandom), heightMap->getHeight(x, z) + 10);

            ring->setPosition(glm::vec3(x, y, z));
        }
    }
}

void Game::renderText(Text characters, Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // activate corresponding render state
    shader.use();
    glUniform3f(glGetUniformLocation(shader.getID(), "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(characters.VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = characters.Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}};
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, characters.VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}

void Game::renderScene(const std::vector<std::shared_ptr<Renderable>> &renderers)
{
    for (const auto &i : renderers)
    {
        i->render();
    }
}

void Game::renderSceneWithShader(const std::vector<std::shared_ptr<Renderable>> &renderers,
                                 const std::shared_ptr<Shader> &shader)
{
    for (const auto &i : renderers)
    {
        i->renderWithShader(shader);
    }
}

void Game::update(float delta, const std::vector<std::shared_ptr<Renderable>> &renderers)
{
    for (const auto &i : renderers)
    {
        i->update(delta);
    }
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
