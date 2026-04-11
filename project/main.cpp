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

	Shader heightMapShader(PATH_TO_SRC "/../assets/shaders/cpu_height.vert", PATH_TO_SRC "/../assets/shaders/cpu_height.frag");
	Shader waterShader(PATH_TO_SRC "/../assets/shaders/water.vert", PATH_TO_SRC "/../assets/shaders/water.frag");
	Shader treeShader(PATH_TO_SRC "/../assets/shaders/tree.vert", PATH_TO_SRC "/../assets/shaders/tree.frag");

	// Objects
	Object tree(PATH_TO_SRC "/../assets/models/Tree.obj");
	Object water(PLAN_SIZE_X / 2, waterHeight, true);

	// Terrain
	TerrainGeneration heightMap(PATH_TO_SRC "/../assets/textures/iceland_heightmap.png", PLAN_SIZE_X, PLAN_SIZE_X);
	TerrainRenderer terrainRenderer(heightMap, heightMapShader);

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

	WaterRenderer waterRenderer(waterShader, water, fbos);

	glm::vec4 reflectionPlane = glm::vec4(0, 1, 0, waterHeight);
	glm::vec4 refractionPlane = glm::vec4(0, -1, 0, waterHeight);

	Shader shader(PATH_TO_SRC "/../assets/shaders/sphere.vert", PATH_TO_SRC "/../assets/shaders/sphere.frag");

	Object sphere1(PATH_TO_SRC "/../assets/models/sphere_smooth.obj");

	sphere1.setWorldPosition(1.0, 15.0, 1.5);
	ObjectRenderer sphereRenderer(shader, &sphere1);

	Light randomLightForSphere(1.0, 15.0, 1.5);
	randomLightForSphere.setProperties(0, 0.4, 1);

	// Rendering

	shader.use();
	shader.setVector3f("materialColour", glm::vec3(1.0f, 1.0, 1.0));
	shader.setLight(randomLightForSphere);

	treeShader.use();
	treeShader.setLight(randomLightForSphere);

	// Texture
	Texture treeTexture(PATH_TO_SRC "/../assets/textures/tree.jpg");

	InstancedRenderer treeRenderer(treeShader, tree, &treeTexture, treeMatrices);

	float skyboxVertices[] = {
		// positions
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f};

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

	std::vector<std::string> faces{
		PATH_TO_SRC "/../assets/textures/cubemaps/skybox/right.jpg",
		PATH_TO_SRC "/../assets/textures/cubemaps/skybox/left.jpg",
		PATH_TO_SRC "/../assets/textures/cubemaps/skybox/top.jpg",
		PATH_TO_SRC "/../assets/textures/cubemaps/skybox/bottom.jpg",
		PATH_TO_SRC "/../assets/textures/cubemaps/skybox/front.jpg",
		PATH_TO_SRC "/../assets/textures/cubemaps/skybox/back.jpg"};

	Cubemap skybox(faces);
	unsigned int cubemapTexture = skybox.getTextureId();

	Shader skyboxShader(PATH_TO_SRC "/../assets/shaders/skybox.vert", PATH_TO_SRC "/../assets/shaders/skybox.frag");
	skyboxShader.use();
	skyboxShader.setInteger("skybox", 0);

	dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

	while (!dm.shouldClose())
	{

		// 1. Reflection
		glEnable(GL_CLIP_DISTANCE0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fbos.bindReflectionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.prepareReflection(waterHeight);
		glBindBuffer(GL_UNIFORM_BUFFER, uboWater);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(reflectionPlane));
		renderScene({&treeRenderer, &terrainRenderer});
		camera.resetCameraAfterReflection(waterHeight);

		// 2. Refraction
		fbos.bindRefractionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindBuffer(GL_UNIFORM_BUFFER, uboWater);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(refractionPlane));

		renderScene({&treeRenderer, &terrainRenderer});

		fbos.unbindCurrentFrameBuffer();
		dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

		// Normal Scene
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glDisable(GL_CLIP_DISTANCE0);

		// draw skybox as last
		glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMatrix4("view", view);
		skyboxShader.setMatrix4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

		heightMapShader.use();
		heightMapShader.setLight(randomLightForSphere); // Send the light data

		renderScene({&treeRenderer, &waterRenderer, &terrainRenderer});

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
