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
#include "game_engine/displaymanager.h"
#include "game_engine/mainCamera.h"
#include "game_engine/renderer/objectRenderer.h"
#include "game_engine/renderer/terrainRenderer.h"
#include "game_engine/waterFrameBuffer.h"
#include "game_engine/renderer/waterRenderer.h"
#include "game_engine/renderer/instancedRenderer.h"

#include "game_engine/skybox.h"
#include "game_engine/renderer/skyboxRenderer.h"

#include "utils/constants.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int PLAN_SIZE_X = 1000;
int waterHeight = 0;

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

	const auto camera = MainCamera::get();

	// Texture
	Texture treeTexture(PATH_TO_SRC "/../assets/textures/tree.jpg");

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

	std::vector<glm::mat4> treeMatrices;

	int maxRandom = PLAN_SIZE_X / 2;
	int minRandom = -PLAN_SIZE_X / 2;
	for (int i = 0; i < 100; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		float x = rand() % (maxRandom - minRandom) + minRandom;
		float z = rand() % (maxRandom - minRandom) + minRandom;
		float y = heightMap.getHeight(x, z);
		if (y <= waterHeight)
		{
			i--;
			continue;
		}
		model = glm::translate(model, glm::vec3(x, y, z));
		model = glm::rotate(model, static_cast<float>(rand() % 360), glm::vec3(0, 1, 0));

		treeMatrices.push_back(model);
	}

	auto reflectionPlane = glm::vec4(0, 1, 0, waterHeight);
	auto refractionPlane = glm::vec4(0, -1, 0, waterHeight);

	auto tree = Object::make(PATH_TO_SRC "/../assets/models/Tree.obj");

	// Renderer
	auto terrainRenderer = std::make_shared<TerrainRenderer>(heightMap);
	auto sphereRenderer = std::make_shared<ObjectRenderer>();
	auto waterRenderer = std::make_shared<WaterRenderer>(fbos);
	auto treeRenderer = std::make_shared<InstancedRenderer>(tree, &treeTexture, treeMatrices);
	auto skyboxRenderer = std::make_shared<SkyboxRenderer>(&skybox);

	WaterFrameBuffer::connectShader(terrainRenderer->getShader());

	// Objects
	auto water = Object::make(PLAN_SIZE_X / 2, waterHeight, waterRenderer);
	auto sphere1 = Object::make(PATH_TO_SRC "/../assets/models/sphere_smooth.obj", sphereRenderer);
	sphere1->setPosition(1.0, 15.0, 1.5);

	auto randomLightForSphere = std::make_shared<Light>();
	randomLightForSphere->setProperties(0.1, 0.9, 1);

	sphere1->attach(randomLightForSphere);

	// Rendering
	sphereRenderer->getShader()->setLight(randomLightForSphere);
	treeRenderer->getShader()->setLight(randomLightForSphere);
	terrainRenderer->getShader()->setLight(randomLightForSphere);

	dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

	while (!dm.shouldClose())
	{
		// 1. Reflection
		glEnable(GL_CLIP_DISTANCE0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fbos->bindReflectionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera->prepareReflection(waterHeight);
		fbos->setClipPlane(reflectionPlane);
		renderScene({treeRenderer, terrainRenderer, skyboxRenderer});
		camera->resetCameraAfterReflection(waterHeight);

		// 2. Refraction
		fbos->bindRefractionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		fbos->setClipPlane(refractionPlane); // One clean call
		renderScene({treeRenderer, terrainRenderer});

		fbos->unbindCurrentFrameBuffer();
		dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

		// Normal Scene
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glDisable(GL_CLIP_DISTANCE0);

		renderScene({treeRenderer, waterRenderer, terrainRenderer, skyboxRenderer});

		double now = glfwGetTime();

		sphereRenderer->getShader()->setVector3f("light.light_pos", randomLightForSphere->getPosition());

		sphereRenderer->render();

		dm.update();
	}
	fbos->cleanUp();
	glfwTerminate();
	return 0;
}
