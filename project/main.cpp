#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils/utils.h"
#include <glm/gtx/color_space.hpp>

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
#include "game_engine/particleGenerator.h"

const int MaxParticles = 10000;
Particle particlesContainer[MaxParticles];

int lastUsedParticle = 0;

int findUnusedParticle()
{
	for (int i = lastUsedParticle; i < MaxParticles; i++)
	{
		if (particlesContainer[i].life < 0)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < lastUsedParticle; i++)
	{
		if (particlesContainer[i].life < 0)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	return 0;
}

void sortParticles()
{
	std::sort(&particlesContainer[0], &particlesContainer[MaxParticles]);
}

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
	for (int i = 0; i < 200; i++)
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

	auto tree = Object::make(PATH_TO_SRC "/../assets/models/Tree_V10_OBJ/Tree.obj");

	// Renderer
	auto terrainRenderer = std::make_shared<TerrainRenderer>(heightMap);
	auto sphereRenderer = std::make_shared<ObjectRenderer>();
	auto waterRenderer = std::make_shared<WaterRenderer>(fbos);
	auto treeRenderer = std::make_shared<InstancedRenderer>(tree, nullptr, treeMatrices);
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

	char fileVert[128] = PATH_TO_SRC "/../assets/shaders/part.vert";
	char fileFrag[128] = PATH_TO_SRC "/../assets/shaders/part.frag";
	Shader shader(fileVert, fileFrag);

	// First object!
	const float vertexData[18] = {
		// vertices
		-1.0, -1.0, 0.0,
		1.0, -1.0, 0.0,
		-1.0, 1.0, 0.0,
		1.0, 1.0, 0.0,
		-1.0, 1.0, 0.0,
		1.0, -1.0, 0.0};

	static GLfloat *g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	static GLfloat *g_particule_color_data = new GLfloat[MaxParticles * 4];

	for (int i = 0; i < MaxParticles; i++)
	{
		particlesContainer[i].life = -1.0;
	}

	// Create the vertex buffer objects for the quad used as a particle,
	// and the positions and colors of all particle
	// the same vertex array object is used for all 3 VBOs
	GLuint VBO_vertex, VBO_position, VBO_color, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_vertex);
	glGenBuffers(1, &VBO_position);
	glGenBuffers(1, &VBO_color);

	// define VBO and VAO as active buffer and active vertex array
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	auto att_vertex = glGetAttribLocation(shader.getID(), "vertex");
	glEnableVertexAttribArray(att_vertex);
	glVertexAttribPointer(att_vertex, 3, GL_FLOAT, false, 0, 0);
	glVertexAttribDivisor(att_vertex, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GL_FLOAT), NULL, GL_STREAM_DRAW);

	auto att_center = glGetAttribLocation(shader.getID(), "center");
	glEnableVertexAttribArray(att_center);
	glVertexAttribPointer(att_center, 4, GL_FLOAT, false, 0, 0);
	glVertexAttribDivisor(att_center, 1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	auto att_col = glGetAttribLocation(shader.getID(), "col");
	glEnableVertexAttribArray(att_col);
	glVertexAttribPointer(att_col, 4, GL_FLOAT, true, 0, 0);
	glVertexAttribDivisor(att_col, 1);

	// desactive the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glm::mat4 view = camera->getViewMatrix();
	glm::mat4 perspective = camera->getProjectionMatrix();

	glm::vec3 cameraRight = camera->getRight();
	glm::vec3 cameraUp = camera->getUp();
	glm::vec3 cameraPosition = camera->getPosition();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	double lastTime = glfwGetTime();

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

		view = camera->getViewMatrix();
		perspective = camera->getProjectionMatrix(45.0, (float)dm.lastX / (float)dm.lastY, 0.01, 100.0);
		cameraRight = camera->getRight();
		cameraUp = camera->getUp();
		cameraPosition = camera->getPosition();
		glfwPollEvents();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;

		// Add new particles

		int newParticle = delta * 1000.0f;
		if (newParticle > (int)(0.032f * 1000.0))
			newParticle = (int)(0.032f * 1000.0);

		for (int i = 0; i < newParticle; i++)
		{
			int particleIdx = findUnusedParticle();
			particlesContainer[particleIdx].life = 15.0f + rand() % 10 / 10.0;
			particlesContainer[particleIdx].pos = glm::vec3(glm::cos(currentTime) * 0.5, 2.0 + rand() % 100 / 1000.0, -5.0f + rand() % 100 / 1000.0);
			particlesContainer[particleIdx].speed = glm::vec3(rand() % 20 / 30.0, 1.0 - rand() % 10 / 20.0, rand() % 100 / 1000.0);

			// use hsv color to get pretty results
			particlesContainer[particleIdx].color = glm::vec4(209.0, 0.07, 0.89, 0.5);

			particlesContainer[particleIdx].size = 0.05f;
		}

		// Simulate the particle
		int particleCount = 0;
		for (int i = 0; i < MaxParticles; i++)
		{
			Particle &p = particlesContainer[i]; // shortcut

			if (p.life > 0.0)
			{
				// decrease life, use time since last frame
				p.life -= delta;

				// change of pos
				p.pos += p.speed * (float)delta;

				p.color.y += (float)delta * 0.5;
				p.color.r += glm::cos(rand()) * 0.5;

				p.color.a -= (float)delta * 0.05;

				// update distance with the camera
				p.cameraDist = glm::length(p.pos - cameraPosition);

				// fill the gpu buffer
				g_particule_position_size_data[4 * particleCount] = p.pos.x;
				g_particule_position_size_data[4 * particleCount + 1] = p.pos.y;
				g_particule_position_size_data[4 * particleCount + 2] = p.pos.z;

				g_particule_position_size_data[4 * particleCount + 3] = p.size;

				glm::vec3 hsv = glm::vec3(p.color.r, p.color.g, p.color.b);
				glm::vec3 rgb = glm::rgbColor(hsv);
				g_particule_color_data[4 * particleCount + 0] = rgb.r;
				g_particule_color_data[4 * particleCount + 1] = rgb.g;
				g_particule_color_data[4 * particleCount + 2] = rgb.b;
				g_particule_color_data[4 * particleCount + 3] = p.color.a;

				particleCount++;
			}

			else
			{
				// make sure all dead particle will be put at the end of the list
				p.cameraDist = -1;
			}
		}

		sortParticles();

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GL_FLOAT), NULL, GL_STREAM_DRAW);
		;
		glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		;
		glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(GLfloat) * 4, g_particule_color_data);

		shader.use();

		shader.setMatrix4("V", view);
		shader.setMatrix4("P", perspective);

		shader.setVector3f("cameraRight", cameraRight);
		shader.setVector3f("cameraUp", cameraUp);

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, particleCount);

		dm.update();
	}
	fbos->cleanUp();
	glfwTerminate();
	return 0;
}
