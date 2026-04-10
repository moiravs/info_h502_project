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
		FATAL("Failed to initialise GLFW \n");
	}

	auto dm = DisplayManager();

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		FATAL("Failed to initialize GLAD");
	}

	glEnable(GL_DEPTH_TEST);

	Shader heightMapShader(PATH_TO_SRC "/../assets/shaders/cpu_height.vert", PATH_TO_SRC "/../assets/shaders/cpu_height.frag");
	Shader waterShader(PATH_TO_SRC "/../assets/shaders/water.vert", PATH_TO_SRC "/../assets/shaders/water.frag");
	Shader treeShader(PATH_TO_SRC "/../assets/shaders/tree.vert", PATH_TO_SRC "/../assets/shaders/tree.frag");

	// Texture
	Texture wall(PATH_TO_SRC "/../assets/textures/wall.jpg");

	// Objects
	Object tree(PATH_TO_SRC "/../assets/models/Tree.obj");
	Object water(PLAN_SIZE_X / 2, waterHeight, true);

	// Terrain
	TerrainGeneration heightMap(PATH_TO_SRC "/../assets/textures/iceland_heightmap.png", PLAN_SIZE_X, PLAN_SIZE_X);
	TerrainRenderer terrainRenderer(heightMap);

	GLuint uboWater;
	glGenBuffers(1, &uboWater);
	glBindBuffer(GL_UNIFORM_BUFFER, uboWater);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	GLuint blockIndex = glGetUniformBlockIndex(heightMapShader.ID, "WaterData");
	glUniformBlockBinding(heightMapShader.ID, blockIndex, 3);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, uboWater);

	heightMapShader.setVector4f("plane", glm::vec4(0, 1, 0, waterHeight));

	WaterFrameBuffers fbos = WaterFrameBuffers();

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

	InstancedRenderer treeRenderer(treeShader, tree, wall, treeMatrices);

	WaterRenderer waterRenderer(waterShader, water, fbos);

	glm::vec4 reflectionPlane = glm::vec4(0, 1, 0, waterHeight);
	glm::vec4 refractionPlane = glm::vec4(0, -1, 0, waterHeight);

	Shader shader(PATH_TO_SRC "/../assets/shaders/sphere.vert", PATH_TO_SRC "/../assets/shaders/sphere.frag");

	Object sphere1(PATH_TO_SRC "/../assets/models/sphere_smooth.obj");

	sphere1.setWorldPosition(0, 20, -2);
	ObjectRenderer sphereRenderer(shader, sphere1);

	Light randomLightForSphere(1.0, 22.0, 1.5);
	randomLightForSphere.setProperties(0.1, 0.5, 0.8);

	// Rendering

	shader.use();
	shader.setVector3f("materialColour", glm::vec3(0.5f, 0.6, 0.8));
	shader.setLight(randomLightForSphere);

	dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

	while (!dm.shouldClose())
	{

		// Reflection
		glEnable(GL_CLIP_DISTANCE0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		fbos.bindReflectionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float distance = 2 * (camera.GetCameraPosition().y - waterHeight);

		camera.SetCameraPosition(glm::vec3(camera.GetCameraPosition().x, camera.GetCameraPosition().y - distance, camera.GetCameraPosition().z));
		camera.invertPitch();

		glBindBuffer(GL_UNIFORM_BUFFER, uboWater);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(reflectionPlane));

		heightMapShader.use();
		heightMapShader.updatePos(camera);
		terrainRenderer.draw();
		treeRenderer.render();

		camera.invertPitch();

		camera.SetCameraPosition(glm::vec3(camera.GetCameraPosition().x, camera.GetCameraPosition().y + distance, camera.GetCameraPosition().z));

		// Refraction
		fbos.bindRefractionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindBuffer(GL_UNIFORM_BUFFER, uboWater);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(refractionPlane));

		heightMapShader.use();
		heightMapShader.updatePos(camera);
		terrainRenderer.draw();
		treeRenderer.render();

		fbos.unbindCurrentFrameBuffer();

		dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

		// Normal
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		glDisable(GL_CLIP_DISTANCE0);

		heightMapShader.use();
		heightMapShader.updatePos(camera);

		terrainRenderer.draw();
		treeRenderer.render();
		waterRenderer.render();

		double now = glfwGetTime();

		shader.use();

		shader.setVector3f("u_view_pos", camera.Position);
		shader.updatePos(camera);

		auto delta = randomLightForSphere.getPos() + glm::vec3(0.0, 0.0, 2 * std::sin(now));
		// std::cout << delta.z <<std::endl;
		shader.setVector3f("light.light_pos", delta);

		sphereRenderer.render();

		dm.update();
	}
	fbos.cleanUp();
	glfwTerminate();
	return 0;
}
