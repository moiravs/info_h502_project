#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include <vector>

#include "game_engine/shader.h"
#include "game_engine/camera.h"
#include "game_engine/terrainGeneration.h"
#include "game_engine/displaymanager.h"
#include "game_engine/objectRenderer.h"
#include "game_engine/terrainRenderer.h"
#include "game_engine/waterFrameBuffers.h"
#include "game_engine/waterRenderer.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int PLAN_SIZE_X = 1000;
int waterHeight = 0;

int main()
{

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	if (!glfwInit())
	{
		throw std::runtime_error("Failed to initialise GLFW \n");
	}

	auto dm = DisplayManager();

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glEnable(GL_DEPTH_TEST);

	// Shaders
	Shader heightMapShader(PATH_TO_SRC "/../assets/shaders/cpu_height.vert", PATH_TO_SRC "/../assets/shaders/cpu_height.frag");
	Shader heightMapReflectionShader(PATH_TO_SRC "/../assets/shaders/cpu_height.vert", PATH_TO_SRC "/../assets/shaders/cpu_height.frag");
	Shader heightMapRefractionShader(PATH_TO_SRC "/../assets/shaders/cpu_height.vert", PATH_TO_SRC "/../assets/shaders/cpu_height.frag");
	Shader waterShader(PATH_TO_SRC "/../assets/shaders/water.vert", PATH_TO_SRC "/../assets/shaders/water.frag");
	Shader treeShader(PATH_TO_SRC "/../assets/shaders/bunny.vert", PATH_TO_SRC "/../assets/shaders/bunnyblue.frag");

	// Texture
	Texture wall(PATH_TO_SRC "/../assets/textures/wall.jpg");

	// Objects
	Object tree(PATH_TO_SRC "/../assets/models/Tree.obj");
	Object water(PLAN_SIZE_X / 2, waterHeight, true);

	// Terrain
	TerrainGeneration heightMap(PATH_TO_SRC "/../assets/textures/iceland_heightmap.png", PLAN_SIZE_X, PLAN_SIZE_X);
	TerrainRenderer terrainRenderer(heightMap);

	heightMapShader.setVector4f("plane", glm::vec4(0, 1, 0, waterHeight));

	heightMapReflectionShader.setVector4f("plane", glm::vec4(0, 1, 0, waterHeight));

	heightMapRefractionShader.setVector4f("plane", glm::vec4(0, -1, 0, waterHeight));

	WaterFrameBuffers fbos = WaterFrameBuffers();

	std::vector<Object> trees;

	// Create 3 trees
	for (int i = 0; i < 3; i++)
	{
		Object tree(PATH_TO_SRC "/../assets/models/Tree.obj");
		trees.emplace_back(tree);
	}

	// Position them (this can be done once or in the update loop)
	trees[0].setWorldPosition(0.0f, 0.0f, heightMap);
	trees[1].setWorldPosition(20.0f, 20.0f, heightMap);
	trees[2].setWorldPosition(40.0f, 0.0f, heightMap);

	ObjectRenderer treeRenderer(treeShader, trees, wall);

	WaterRenderer waterRenderer(waterShader, water, fbos);

	dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

	while (!dm.shouldClose())
	{

		// Reflection
		glEnable(GL_CLIP_DISTANCE0);
		// fbos->bindReflectionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Essential!

		fbos.bindReflectionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Essential!
		float distance = 2 * (camera.GetCameraPosition().y - waterHeight);

		camera.SetCameraPosition(glm::vec3(camera.GetCameraPosition().x, camera.GetCameraPosition().y - distance, camera.GetCameraPosition().z));
		heightMapReflectionShader.use();
		camera.invertPitch();
		heightMapReflectionShader.updatePos(camera);
		terrainRenderer.draw();

		camera.invertPitch();

		camera.SetCameraPosition(glm::vec3(camera.GetCameraPosition().x, camera.GetCameraPosition().y + distance, camera.GetCameraPosition().z));

		// Refraction
		fbos.bindRefractionFrameBuffer();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Essential!
		heightMapRefractionShader.use();
		heightMapRefractionShader.updatePos(camera);
		terrainRenderer.draw();
		fbos.unbindCurrentFrameBuffer();

		dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

		// Normal
		glDisable(GL_CLIP_DISTANCE0);

		heightMapShader.use();
		heightMapShader.updatePos(camera);

		terrainRenderer.draw();
		treeRenderer.render();
		waterRenderer.render();

		dm.update();
	}
	fbos.cleanUp();
	glfwTerminate();
	return 0;
}
