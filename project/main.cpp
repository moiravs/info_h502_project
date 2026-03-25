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

	Texture heightMapTexture(PATH_TO_SRC "/../assets/textures/iceland_heightmap.png", PLAN_SIZE_X, PLAN_SIZE_X);

	Object water(PLAN_SIZE_X / 2, true);
	water.makeObject(waterShader);

	int framebuffer_width, framebuffer_height;
	glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
	glViewport(0, 0, framebuffer_width, framebuffer_height);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		dm.processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		heightMapShader.use();
		heightMapShader.updatePos();
		heightMapTexture.draw();

		waterShader.use();
		waterShader.updatePos();
		water.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
