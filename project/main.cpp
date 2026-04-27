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
#include "game_engine/manager/displaymanager.h"
#include "game_engine/mainCamera.h"
#include "game_engine/waterFrameBuffer.h"

#include "game_engine/entity/skybox.h"
#include "game_engine/manager/lightManager.h"
#include "game_engine/renderer/skyboxRenderer.h"

#include "utils/constants.h"
#include "game_engine/prop/propMaker.h"
#include "game_engine/game.h"
#include "game_engine/entity/particleGenerator.h"
#include "game_engine/entity/renderableEntityMaker.h"
#include "game_engine/renderer/waterRenderer.h"

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
	auto heightMap =
		TerrainMesh::terrainFromTexture(PATH_TO_SRC "/../assets/textures/iceland_heightmap.png", PLAN_SIZE_X, PLAN_SIZE_X);

	// Skybox
	auto skybox = RenderableEntityMaker::makeRenderable<Skybox, SkyboxRenderer>(
		"skybox", std::vector<std::string>{PATH_TO_SRC "/../assets/textures/cubemaps/skybox/right.jpg",
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
	// auto skyboxRenderer = std::make_shared<SkyboxRenderer>(&skybox);

	// Objects
	auto water = RenderableEntityMaker::makeRenderable<Object, WaterRenderer>(
		fbos, Mesh::createPlane(PLAN_SIZE_X / 2, WATER_HEIGHT));

	auto redLight = PropMaker::makeLamp(
		glm::vec3(1.0, 15.0, 1.5), glm::vec3(1), glm::vec3(1, 0, 0),
		glm::vec4(0.1, 0.9, 1, 32), glm::vec3(0.5, 0.01, 0));

	auto terrain = Object::make(heightMap, "cpu_height");

	// the white light is fixed to the camera (it's not rendered)
	// camera->attach(whiteLight->getMainObject(), glm::vec3(0, 0, 0));
	auto sun = PropMaker::makeLamp(
		glm::vec3(.0, 100.0, 1.5), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1),
		glm::vec4(0, 0.9, 1, 32), glm::vec3(0.005, 0.005, 0));

	auto plane = PropMaker::makePlane(heightMap);

	dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

	auto trees = PropMaker::makeTrees(heightMap);
	auto &lightManager = LightManager::get();

	auto pg = ParticleGenerator::make(ParticleParams{
		.spread = 0.2,
		.range = 0.5,
		.initialSize = 0.1,
		.maxLife = 2,
		.color1 = glm::vec3(1.0f, 1.0f, 0.8f),
		.color2 = glm::vec3(1.0f, 0.5f, 0.0f),
		.color3 = glm::vec3(0.5f, 0.0f, 0.0f)});
	pg->setPosition(glm::vec3(0.5, heightMap->getHeight(0.5, -5.0), -5));

	float orbitRadius = PLAN_SIZE_X / 2; // Distance from the center of the scene
	float orbitSpeed = 0.1f;			 // How fast the sun moves
	float orbitHeight = 100.0f;			 // Vertical height of the sun

	auto firecamp = PropMaker::makeFirecamp(heightMap);

	double lastTime = glfwGetTime();

	auto game = Game();

	while (!dm.shouldClose())
	{
		const double currentTime = glfwGetTime();
		const double delta = currentTime - lastTime;
		glEnable(GL_DEPTH_TEST);
		lastTime = currentTime;

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

		Game::renderScene(delta, {trees, skybox, terrain});
		camera->resetCameraAfterReflection(WATER_HEIGHT);
		camera->updateUBO();

		// 2. Refraction
		fbos->bindRefractionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		fbos->setClipPlane(refractionPlane); // One clean call
		Game::renderScene(delta, {trees, terrain});

		fbos->unbindCurrentFrameBuffer();
		dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

		// Normal Scene
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glDisable(GL_CLIP_DISTANCE0);

		float sunX = 0.0f;
		float sunY = std::sin(currentTime * orbitSpeed) * orbitRadius;
		float sunZ = std::cos(currentTime * orbitSpeed) * orbitRadius;

		sun->getMainObject()->setPosition(glm::vec3(sunX, sunY, sunZ));

		Game::renderScene(delta, {pg, trees, redLight, water, skybox, terrain, sun, firecamp, plane});
		dm.update();
	}

	glfwTerminate();
	return 0;
}
