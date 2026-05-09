#ifndef Game_H
#define Game_H

#include <vector>

#include "entity/renderableEntity.h"
#include "entity/text.h"
#include "shader.h"


class DepthMap;
class HeightMap;

class Game
{

public:
    Game();
    static void renderScene(const std::vector<std::shared_ptr<Renderable>> &renderers);
    static void update(float delta, const std::vector<std::shared_ptr<Renderable>> &renderers);
    static void renderSceneWithShader(const std::vector<std::shared_ptr<Renderable>> &renderers, const std::shared_ptr<Shader> &shader);
    static void checkTerrainCollision(const std::shared_ptr<Entity> &entity, const std::shared_ptr<HeightMap> &heightMap);
    static void renderText(Text characters, Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);
};

#endif