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

	Shader heightMapShader(PATH_TO_SRC "/../assets/shaders/cpu_height.vs", PATH_TO_SRC "/../assets/shaders/cpu_height.fs");

	int waterHeight = 0;
	Object water(PLAN_SIZE_X / 2, waterHeight, true);

	heightMapShader.use(); // Ensure glUseProgram is called first
	heightMapShader.setVector4f("plane", glm::vec4(0, 1, 0, waterHeight));

	Shader heightMapReflectionShader(PATH_TO_SRC "/../assets/shaders/cpu_height.vs", PATH_TO_SRC "/../assets/shaders/cpu_height.fs");

	heightMapReflectionShader.use(); // Ensure glUseProgram is called first
	heightMapReflectionShader.setVector4f("plane", glm::vec4(0, 1, 0, waterHeight));

	Shader heightMapRefractionShader(PATH_TO_SRC "/../assets/shaders/cpu_height.vs", PATH_TO_SRC "/../assets/shaders/cpu_height.fs");

	heightMapRefractionShader.use(); // Ensure glUseProgram is called first
	heightMapRefractionShader.setVector4f("plane", glm::vec4(0, -1, 0, waterHeight));

	Shader waterShader(PATH_TO_SRC "/../assets/shaders/water.vert", PATH_TO_SRC "/../assets/shaders/water.frag");
	waterShader.use();
	waterShader.setInteger("reflectionTexture", 0);
	waterShader.setInteger("refractionTexture", 1);

	WaterFrameBuffers *fbos = new WaterFrameBuffers();
	glm::mat4 model = glm::mat4(1.0f);

	std::vector<glm::mat4> modelwater = {model};

	WaterRenderer waterRenderer(waterShader, water, modelwater, *fbos);

	Shader treeShader(PATH_TO_SRC "/../assets/shaders/bunny.vert", PATH_TO_SRC "/../assets/shaders/bunnyblue.frag");

	TerrainGeneration heightMapTexture(PATH_TO_SRC "/../assets/textures/iceland_heightmap.png", PLAN_SIZE_X, PLAN_SIZE_X);
	TerrainRenderer island(heightMapTexture);

	Object tree(PATH_TO_SRC "/../assets/models/Tree.obj");

	float y = heightMapTexture.getHeight(0, 0);
	glm::mat4 modelgreen = glm::translate(model, glm::vec3(0, y, 0)); // y is up
	float y2 = heightMapTexture.getHeight(20, 20);
	glm::mat4 modelblue = glm::translate(model, glm::vec3(20, y2, 20));

	float y3 = heightMapTexture.getHeight(40, 0);
	glm::mat4 modelred = glm::translate(model, glm::vec3(40, y3, 0));

	std::vector<glm::mat4> models = {modelblue, modelgreen, modelred};

	Shader texshader(PATH_TO_SRC "/../assets/shaders/tex.frag", PATH_TO_SRC "/../assets/shaders/tex.vert");

	Texture wall(PATH_TO_SRC "/../assets/textures/wall.jpg");

	ObjectRenderer treeRenderer(treeShader, tree, models, wall);

	dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

	while (!dm.shouldClose())
	{

		// Reflection
		glEnable(GL_CLIP_DISTANCE0);
		// fbos->bindReflectionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Essential!
		float distance = 2 * (camera.GetCameraPosition().y - waterHeight);

		camera.SetCameraPosition(glm::vec3(camera.GetCameraPosition().x, camera.GetCameraPosition().y - distance, camera.GetCameraPosition().z));
		heightMapReflectionShader.use();
		camera.invertPitch();
		heightMapReflectionShader.updatePos(camera);
		island.draw();

		camera.SetCameraPosition(glm::vec3(camera.GetCameraPosition().x, camera.GetCameraPosition().y + distance, camera.GetCameraPosition().z));
		camera.invertPitch();
		// fbos->unbindCurrentFrameBuffer();

		fbos->bindReflectionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Essential!
		distance = 2 * (camera.GetCameraPosition().y - waterHeight);

		camera.SetCameraPosition(glm::vec3(camera.GetCameraPosition().x, camera.GetCameraPosition().y - distance, camera.GetCameraPosition().z));
		heightMapReflectionShader.use();
		camera.invertPitch();
		heightMapReflectionShader.updatePos(camera);
		island.draw();

		camera.invertPitch();

		camera.SetCameraPosition(glm::vec3(camera.GetCameraPosition().x, camera.GetCameraPosition().y + distance, camera.GetCameraPosition().z));
		fbos->unbindCurrentFrameBuffer();

		// // Refraction
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Essential!
		// texshader.use();
		// heightMapRefractionShader.use();
		// heightMapRefractionShader.updatePos(camera);
		// island.draw();
		// treeRenderer.render();
		dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

		// Normal
		glDisable(GL_CLIP_DISTANCE0);
		texshader.use();

		heightMapShader.use();
		heightMapShader.updatePos(camera);

		island.draw();
		treeRenderer.render();

		waterRenderer.render();

		dm.update();
	}
	fbos->cleanUp();
	glfwTerminate();
	return 0;
}
