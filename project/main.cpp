#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <vector>

#include "game_engine/shader.h"
#include "game_engine/camera.h"
#include "game_engine/terrainGeneration.h"
#include "game_engine/displaymanager.h"
#include "game_engine/objectRenderer.h"
#include "game_engine/terrainRenderer.h"

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
	Shader waterShader(PATH_TO_SRC "/../assets/shaders/water.vert", PATH_TO_SRC "/../assets/shaders/water.frag");
	Shader treeShader(PATH_TO_SRC "/../assets/shaders/bunny.vert", PATH_TO_SRC "/../assets/shaders/bunnyblue.frag");

	TerrainGeneration heightMapTexture(PATH_TO_SRC "/../assets/textures/iceland_heightmap.png", PLAN_SIZE_X, PLAN_SIZE_X);
	TerrainRenderer island(heightMapTexture);

	Object water(PLAN_SIZE_X / 2, 0, true);
	glm::mat4 model = glm::mat4(1.0f);
	std::vector<glm::mat4> modelwater = {model};

	ObjectRenderer waterRenderer(waterShader, water, modelwater);

	Object tree(PATH_TO_SRC "/../assets/models/Tree.obj");

	float y = heightMapTexture.getHeight(0, 0);
	glm::mat4 modelgreen = glm::translate(model, glm::vec3(0, y, 0)); // y is up
	float y2 = heightMapTexture.getHeight(20, 20);
	glm::mat4 modelblue = glm::translate(model, glm::vec3(20, y2, 20));

	float y3 = heightMapTexture.getHeight(40, 0);
	glm::mat4 modelred = glm::translate(model, glm::vec3(40, y3, 0));

	std::vector<glm::mat4> models = {modelblue, modelgreen, modelred};

	ObjectRenderer treeRenderer(treeShader, tree, models);

	dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

	while (!dm.shouldClose())
	{

		heightMapShader.use();
		heightMapShader.updatePos(camera);
		island.draw();

		waterRenderer.render();

		treeRenderer.render();

		dm.update();
	}

	glfwTerminate();
	return 0;
}
