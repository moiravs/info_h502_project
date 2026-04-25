#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils/utils.h"

#define STB_IMAGE_IMPLEMENTATION
// the include below must NOT be removed
#include <stb_image.h>

#include <vector>

#include "game_engine/shader.h"
#include "game_engine/entity/light.h"
#include "game_engine/entity/camera.h"
#include "game_engine/terrainGeneration.h"
#include "game_engine/manager/displaymanager.h"
#include "game_engine/mainCamera.h"
#include "game_engine/renderer/terrainRenderer.h"
#include "game_engine/waterFrameBuffer.h"
#include "game_engine/renderer/waterRenderer.h"

#include "game_engine/skybox.h"
#include "game_engine/manager/lightManager.h"
#include "game_engine/renderer/skyboxRenderer.h"

#include "utils/constants.h"
#include "game_engine/renderer/particleRenderer.h"
#include "game_engine/prop/propMaker.h"
#include "game_engine/game.h"
#include "game_engine/culling/octree.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void renderScene(const std::vector<std::shared_ptr<Renderer>> &renderers)
{
	for (const auto &i : renderers)
	{
		i->render();
	}
}

int main()
{

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	if (!glfwInit())
	{
		FATAL("Failed to initialise GLFW \n");
	}

	auto dm = DisplayManager();

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		FATAL("Failed to initialize GLAD");
	}

	glEnable(GL_DEPTH_TEST);

	auto camera = MainCamera::get();

	// Terrain
	TerrainGeneration heightMap(PATH_TO_SRC "/../assets/textures/iceland_heightmap.png", PLAN_SIZE_X, PLAN_SIZE_X);

	// Skybox
	Skybox skybox({PATH_TO_SRC "/../assets/textures/cubemaps/skybox/right.jpg",
				   PATH_TO_SRC "/../assets/textures/cubemaps/skybox/left.jpg",
				   PATH_TO_SRC "/../assets/textures/cubemaps/skybox/top.jpg",
				   PATH_TO_SRC "/../assets/textures/cubemaps/skybox/bottom.jpg",
				   PATH_TO_SRC "/../assets/textures/cubemaps/skybox/front.jpg",
				   PATH_TO_SRC "/../assets/textures/cubemaps/skybox/back.jpg"});

	// Water
	auto fbos = std::make_shared<WaterFrameBuffer>();

	auto reflectionPlane = glm::vec4(0, 1, 0, WATER_HEIGHT);
	auto refractionPlane = glm::vec4(0, -1, 0, WATER_HEIGHT);

	// Renderer
	auto terrainRenderer = std::make_shared<TerrainRenderer>(heightMap);
	auto waterRenderer = std::make_shared<WaterRenderer>(fbos);
	auto skyboxRenderer = std::make_shared<SkyboxRenderer>(&skybox);

	// Objects
	auto water = Object::make(PLAN_SIZE_X / 2, WATER_HEIGHT, waterRenderer);

	auto whiteLight = PropMaker::makeLamp(
		glm::vec3(1.0, 15.0, 1.5), 1, glm::vec3(1, 1, 1),
		glm::vec4(0.1, 0.9, 1, 32), glm::vec3(0.5, 0.01, 0));

	auto redLight = PropMaker::makeLamp(
		glm::vec3(1.0, 15.0, 1.5), 1, glm::vec3(1, 0, 0),
		glm::vec4(0.1, 0.9, 1, 32), glm::vec3(0.5, 0.01, 0));

	// the white light is fixed to the camera (it's not rendered)
	camera->attach(whiteLight->getMainObject(), glm::vec3(0, 0, 0));

	dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

	auto trees = PropMaker::makeTrees(heightMap);
	auto &lightManager = LightManager::get();

	auto pg = std::make_shared<ParticleRenderer>(ParticleParams{
		.spawnPoint = glm::vec3(0.5, heightMap.getHeight(0.5, 5.0), -5),
		.spread = 0.2,
		.range = 0.5,
		.initialSize = 0.1,
		.maxLife = 2,
		.color1 = glm::vec3(1.0f, 1.0f, 0.8f),
		.color2 = glm::vec3(1.0f, 0.5f, 0.0f),
		.color3 = glm::vec3(0.5f, 0.0f, 0.0f)});

	double lastTime = glfwGetTime();

	auto game = Game();
	while (!dm.shouldClose())
	{
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;
		pg->update(delta);

		lightManager.updateUBO();
		camera->updateUBO();

		// 1. Reflection
		glEnable(GL_CLIP_DISTANCE0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fbos->bindReflectionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera->prepareReflection(WATER_HEIGHT);
		camera->updateUBO();
		fbos->setClipPlane(reflectionPlane);
		game.renderScene({trees, terrainRenderer, skyboxRenderer});
		camera->resetCameraAfterReflection(WATER_HEIGHT);
		camera->updateUBO();

		// 2. Refraction
		fbos->bindRefractionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		fbos->setClipPlane(refractionPlane); // One clean call
		game.renderScene({trees, terrainRenderer});

		fbos->unbindCurrentFrameBuffer();
		dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

		// Normal Scene
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glDisable(GL_CLIP_DISTANCE0);

		game.renderScene({trees, pg, redLight, waterRenderer, terrainRenderer, skyboxRenderer});
		dm.update();
	}

	glfwTerminate();
	return 0;
}
