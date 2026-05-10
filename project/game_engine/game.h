#ifndef Game_H
#define Game_H

#include <vector>

#include "entity/renderableEntity.h"
#include "entity/text.h"
#include "shader.h"
#include "prop/prop.h"
#include "SFML/Audio.hpp"

class DepthMap;
class HeightMap;

class Game
{

    sf::Music success;

public:
    int numberOfRings = 0;

    Game();
    void checkIfPlaneInRing(const std::shared_ptr<Prop> plane, const std::shared_ptr<Prop> rings, const std::shared_ptr<HeightMap> &heightMap);
    static void renderScene(float delta, const std::vector<std::shared_ptr<Renderable>> &renderers);
    static void checkTerrainCollision(const std::shared_ptr<Entity> &entity, const std::shared_ptr<HeightMap> &heightMap);
    static void renderShadows(const std::vector<std::shared_ptr<Renderable>> &renderables, const std::shared_ptr<DepthMap> &depthMap);
    static void renderText(Text characters, Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);
};

#endif