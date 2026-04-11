#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include <vector>

#include "game_engine/shader.h"
#include "game_engine/light.h"
#include "game_engine/camera.h"
#include "game_engine/terrainGeneration.h"
#include "game_engine/displaymanager.h"
#include "game_engine/objectRenderer.h"
#include "game_engine/terrainRenderer.h"
#include "game_engine/waterFrameBuffers.h"
#include "game_engine/waterRenderer.h"
#include "game_engine/instanceRenderer.h"

#include "utils/utils.h"
#include "game_engine/light.h"
#include "game_engine/cubemap.h"
#include "game_engine/skybox.h"
#include "game_engine/skyboxRenderer.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int PLAN_SIZE_X = 1000;
int waterHeight = 0;

void renderScene(std::vector<Renderer *> renderers)
{
	for (auto i : renderers)
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

	// Shaders
	Shader heightMapShader(PATH_TO_SRC "/../assets/shaders/cpu_height.vert", PATH_TO_SRC "/../assets/shaders/cpu_height.frag");
	Shader waterShader(PATH_TO_SRC "/../assets/shaders/water.vert", PATH_TO_SRC "/../assets/shaders/water.frag");
	Shader treeShader(PATH_TO_SRC "/../assets/shaders/tree.vert", PATH_TO_SRC "/../assets/shaders/tree.frag");
	Shader skyboxShader(PATH_TO_SRC "/../assets/shaders/skybox.vert", PATH_TO_SRC "/../assets/shaders/skybox.frag");
	Shader shader(PATH_TO_SRC "/../assets/shaders/sphere.vert", PATH_TO_SRC "/../assets/shaders/sphere.frag");

	// Texture
	Texture treeTexture(PATH_TO_SRC "/../assets/textures/tree.jpg");

	// Objects
	Object tree(PATH_TO_SRC "/../assets/models/Tree.obj");
	Object water(PLAN_SIZE_X / 2, waterHeight, true);
	Object sphere1(PATH_TO_SRC "/../assets/models/sphere_smooth.obj");

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
	WaterFrameBuffers fbos = WaterFrameBuffers();
	fbos.connectShader(heightMapShader);

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
		model = glm::rotate(model, (float)(rand() % 360), glm::vec3(0, 1, 0));

		treeMatrices.push_back(model);
	}

	glm::vec4 reflectionPlane = glm::vec4(0, 1, 0, waterHeight);
	glm::vec4 refractionPlane = glm::vec4(0, -1, 0, waterHeight);

	sphere1.setWorldPosition(1.0, 15.0, 1.5);

	Light randomLightForSphere(1.0, 15.0, 1.5);
	randomLightForSphere.setProperties(0, 0.4, 1);

	// Rendering
	skyboxShader.use();
	skyboxShader.setInteger("skybox", 0);

	shader.use();
	shader.setVector3f("materialColour", glm::vec3(1.0f, 1.0, 1.0));
	shader.setLight(randomLightForSphere);

	treeShader.use();
	treeShader.setLight(randomLightForSphere);

	// Renderer
	TerrainRenderer terrainRenderer(heightMap, heightMapShader);
	ObjectRenderer sphereRenderer(shader, &sphere1);
	WaterRenderer waterRenderer(waterShader, water, fbos);
	InstancedRenderer treeRenderer(treeShader, tree, &treeTexture, treeMatrices);
	SkyboxRenderer skyboxRenderer(skyboxShader, &skybox);

	dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

	while (!dm.shouldClose())
	{

		// 1. Reflection
		glEnable(GL_CLIP_DISTANCE0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fbos.bindReflectionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.prepareReflection(waterHeight);
		fbos.setClipPlane(reflectionPlane);
		renderScene({&treeRenderer, &terrainRenderer, &skyboxRenderer});
		camera.resetCameraAfterReflection(waterHeight);

		// 2. Refraction
		fbos.bindRefractionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		fbos.setClipPlane(refractionPlane); // One clean call
		renderScene({&treeRenderer, &terrainRenderer});

		fbos.unbindCurrentFrameBuffer();
		dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

		// Normal Scene
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glDisable(GL_CLIP_DISTANCE0);

		heightMapShader.use();
		heightMapShader.setLight(randomLightForSphere); // Send the light data

		renderScene({&treeRenderer, &waterRenderer, &terrainRenderer, &skyboxRenderer});

		double now = glfwGetTime();

		shader.use();
		shader.setVector3f("u_view_pos", camera.Position);
		shader.updatePos(camera);

		auto delta = randomLightForSphere.getPos() + glm::vec3(0.0, 0.0, 2 * std::sin(now));
		// std::cout << delta.z <<std::endl;
		sphere1.setWorldPosition(delta.x, delta.y, delta.z);
		randomLightForSphere.setPos(delta);
		shader.setVector3f("light.light_pos", delta);

		sphereRenderer.render();

		dm.update();
	}
	fbos.cleanUp();
	glfwTerminate();
	return 0;
}
