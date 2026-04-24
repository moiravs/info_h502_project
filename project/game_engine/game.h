#ifndef Game_H
#define Game_H

#include <string>
#include <sstream>
#include <vector>

#include <vector>

#include "shader.h"
#include "entity/light.h"
#include "entity/camera.h"
#include "terrainGeneration.h"
#include "manager/displaymanager.h"
#include "mainCamera.h"
#include "renderer/objectRenderer.h"
#include "renderer/terrainRenderer.h"
#include "waterFrameBuffer.h"
#include "renderer/waterRenderer.h"
#include "renderer/instancedRenderer.h"

#include "../utils/utils.h"
#include "skybox.h"
#include "manager/lightManager.h"
#include "renderer/skyboxRenderer.h"

#include "particleGenerator.h"
#include "fireGenerator.h"

class Game
{

public:
    Game();
    void renderScene(const std::vector<std::shared_ptr<Renderer>> &renderers);
    void checkTerrainCollision(std::shared_ptr<Entity> entity, TerrainGeneration terrain);
};

#endif