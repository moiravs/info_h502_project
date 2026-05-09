#include "game.h"

#include "depth/depthMap.h"
#include "prop/prop.h"
#include "renderer/meshRenderer.h"

#include "mesh/heightMap.h"
#include "entity/text.h"
#include <map>

Game::Game()
{
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
