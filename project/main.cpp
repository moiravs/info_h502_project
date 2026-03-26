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
#include "game_engine/texture.h"
#include "game_engine/displaymanager.h"
#include "game_engine/object.h"

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

	DisplayManager dm = DisplayManager();
	GLFWwindow *window = dm.createWindow();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glEnable(GL_DEPTH_TEST);

	Shader heightMapShader(PATH_TO_SRC "/../assets/shaders/cpu_height.vs", PATH_TO_SRC "/../assets/shaders/cpu_height.fs");
	Shader waterShader(PATH_TO_SRC "/../assets/shaders/water.vert", PATH_TO_SRC "/../assets/shaders/water.frag");
	Shader treeblueShader(PATH_TO_SRC "/../assets/shaders/bunny.vert", PATH_TO_SRC "/../assets/shaders/bunnyblue.frag");
	Shader treeredShader(PATH_TO_SRC "/../assets/shaders/bunny.vert", PATH_TO_SRC "/../assets/shaders/bunnyred.frag");
	Shader treegreenShader(PATH_TO_SRC "/../assets/shaders/bunny.vert", PATH_TO_SRC "/../assets/shaders/bunnygreen.frag");

	Texture heightMapTexture(PATH_TO_SRC "/../assets/textures/iceland_heightmap.png", PLAN_SIZE_X, PLAN_SIZE_X);

	Object water(PLAN_SIZE_X / 2, true);
	water.makeObject(waterShader);

	Object treered(PATH_TO_SRC "/../assets/models/Tree.obj");
	Object treeblue(PATH_TO_SRC "/../assets/models/Tree.obj");
	Object treegreen(PATH_TO_SRC "/../assets/models/Tree.obj");
	treeblue.makeObject(treeblueShader);
	treered.makeObject(treeredShader);
	treegreen.makeObject(treegreenShader);

	int framebuffer_width,
		framebuffer_height;
	glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
	glViewport(0, 0, framebuffer_width, framebuffer_height);

	while (!glfwWindowShouldClose(window))
	{

		heightMapShader.use();
		heightMapShader.updatePos(camera);
		heightMapTexture.draw();

		waterShader.use();
		waterShader.updatePos(camera);
		water.draw();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 modelgreen = glm::mat4(1.0f);
		glm::mat4 modelred = glm::mat4(1.0f);
		glm::mat4 modelblue = glm::mat4(1.0f);

		float y = heightMapTexture.getHeight(0, 0);
		modelgreen = glm::translate(model, glm::vec3(0, y, 0)); // y is up
		float y2 = heightMapTexture.getHeight(20, 20);
		modelblue = glm::translate(model, glm::vec3(20, y2, 20));

		float y3 = heightMapTexture.getHeight(40, 0);
		modelred = glm::translate(model, glm::vec3(40, y3, 0));

		treeblueShader.use();
		treeblueShader.updatePos(camera, modelblue);
		treeblue.draw();

		treeredShader.use();
		treeredShader.updatePos(camera, modelred);
		treered.draw();

		treegreenShader.use();
		treegreenShader.updatePos(camera, modelgreen);
		treegreen.draw();

		dm.update(window);
	}

	glfwTerminate();
	return 0;
}
