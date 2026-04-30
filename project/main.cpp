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
#include "game_engine/manager/displaymanager.h"
#include "game_engine/mainCamera.h"
#include "game_engine/waterFrameBuffer.h"

#include "game_engine/entity/skybox.h"
#include "game_engine/manager/lightManager.h"
#include "game_engine/renderer/skyboxRenderer.h"

#include "utils/constants.h"
#include "game_engine/prop/propMaker.h"
#include "game_engine/game.h"
#include "game_engine/entity/particleGenerator.h"
#include "game_engine/entity/renderableEntityMaker.h"
#include "game_engine/renderer/waterRenderer.h"
#include <glm/gtx/string_cast.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

unsigned int debugQuadVAO = 0;
unsigned int debugQuadVBO;
void renderDebugQuad()
{
	if (debugQuadVAO == 0)
	{
		// Positions (x, y, z) and TexCoords (u, v)
		// This renders a square from 0.3 to 1.0 on the screen (bottom right)
		float quadVertices[] = {
			// positions        // texcoords
			0.3f,
			1.0f,
			0.0f,
			0.0f,
			1.0f, // Top Left
			0.3f,
			0.3f,
			0.0f,
			0.0f,
			0.0f, // Bottom Left
			1.0f,
			1.0f,
			0.0f,
			1.0f,
			1.0f, // Top Right
			1.0f,
			0.3f,
			0.0f,
			1.0f,
			0.0f, // Bottom Right
		};
		glGenVertexArrays(1, &debugQuadVAO);
		glGenBuffers(1, &debugQuadVBO);
		glBindVertexArray(debugQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, debugQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

		// Position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
		// TexCoord attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	}
	glBindVertexArray(debugQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
#define CONTROL_CAMERA 1

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

	// make sun must be in first
	auto sun = PropMaker::makeLamp(
		glm::vec3(.0, 100.0, 1.5), glm::vec3(10, 10, 10), glm::vec3(1, 1, 1),
		glm::vec4(0, 0.9, 1, 32), glm::vec3(0.005, 0.005, 0));

	auto camera = MainCamera::get();
	// Terrain
	auto heightMap =
		TerrainMesh::terrainFromTexture(PATH_TO_SRC "/../assets/textures/iceland_heightmap.png", PLAN_SIZE_X, PLAN_SIZE_X);

	// Skybox
	auto skybox = RenderableEntityMaker::makeRenderable<Skybox, SkyboxRenderer>(
		"skybox", std::vector<std::string>{PATH_TO_SRC "/../assets/textures/cubemaps/skybox/right.jpg",
										   PATH_TO_SRC "/../assets/textures/cubemaps/skybox/left.jpg",
										   PATH_TO_SRC "/../assets/textures/cubemaps/skybox/top.jpg",
										   PATH_TO_SRC "/../assets/textures/cubemaps/skybox/bottom.jpg",
										   PATH_TO_SRC "/../assets/textures/cubemaps/skybox/front.jpg",
										   PATH_TO_SRC "/../assets/textures/cubemaps/skybox/back.jpg"});

	// Water
	auto fbos = std::make_shared<WaterFrameBuffer>();

	auto reflectionPlane = glm::vec4(0, 1, 0, WATER_HEIGHT);
	auto refractionPlane = glm::vec4(0, -1, 0, WATER_HEIGHT);

	// Renderer
	// auto skyboxRenderer = std::make_shared<SkyboxRenderer>(&skybox);

	// Objects
	auto water = RenderableEntityMaker::makeRenderable<Object, WaterRenderer>(
		fbos, Mesh::createPlane(PLAN_SIZE_X / 2, WATER_HEIGHT));

	auto redLight = PropMaker::makeLamp(
		glm::vec3(1.0, 15.0, 1.5), glm::vec3(1), glm::vec3(1, 1, 1),
		glm::vec4(0.1, 0.9, 1, 32), glm::vec3(0.5, 0.01, 0));

	auto terrain = Object::make(heightMap, "cpu_height");

	auto plane = PropMaker::makePlane(heightMap);

	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

	auto trees = PropMaker::makeTrees(heightMap);
	auto &lightManager = LightManager::get();

	float orbitRadius = PLAN_SIZE_X / 2; // Distance from the center of the scene
	float orbitSpeed = 0.1f;			 // How fast the sun moves
	float orbitHeight = 100.0f;			 // Vertical height of the sun

	auto firecamp = PropMaker::makeFirecamp(5, 0, heightMap);

	double lastTime = glfwGetTime();

	const std::shared_ptr<Shader> shadowShader = std::make_shared<Shader>(PATH_TO_SRC "/../assets/shaders/shadow.vert", PATH_TO_SRC "/../assets/shaders/shadow.frag");

	auto game = Game();
	plane->getMainObject()->rotate(0, 0, 0);

	while (!dm.shouldClose())
	{
		const double currentTime = glfwGetTime();
		const double delta = currentTime - lastTime;
		glEnable(GL_DEPTH_TEST);
		lastTime = currentTime;

		lightManager.updateUBO();
		camera->updateUBO();

		glm::vec3 target = glm::vec3(0.0f, 50.0f, 0.0f); // Terrain center, some height
		glm::mat4 lightView = glm::lookAt(sun->getMainObject()->getPosition(), target, glm::vec3(0.0f, 1.0f, 0.0f));

		float orthoSize = PLAN_SIZE_X * 0.7f;
		float lightNear = 0.1f; // Just in front of the "sun"
		float lightFar = 1000.0f;
		glm::mat4 lightProjection = glm::ortho(-300.0f, 300.0f, -100.0f, 400.0f,
											   lightNear, lightFar);

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		shadowShader->use();
		shadowShader->setMatrix4("lightSpaceMatrix", lightSpaceMatrix);
		Game::renderShadows(delta, {trees, plane, terrain}, lightSpaceMatrix, depthMap, shadowShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK); // Reactivate color drawing
		glReadBuffer(GL_BACK);
		dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

		// 1. Reflection
		glEnable(GL_CLIP_DISTANCE0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fbos->bindReflectionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera->prepareReflection(WATER_HEIGHT);
		camera->updateUBO();
		fbos->setClipPlane(reflectionPlane);

		Game::renderScene(delta, {trees, skybox, terrain}, lightSpaceMatrix, depthMap);
		camera->resetCameraAfterReflection(WATER_HEIGHT);
		camera->updateUBO();

		// 2. Refraction
		fbos->bindRefractionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		fbos->setClipPlane(refractionPlane); // One clean call
		Game::renderScene(delta, {trees, terrain}, lightSpaceMatrix, depthMap);

		fbos->unbindCurrentFrameBuffer();
		dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

		// Normal Scene
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glDisable(GL_CLIP_DISTANCE0);

		float sunX = 0.0f;
		float sunY = std::sin(currentTime * orbitSpeed) * orbitRadius;
		float sunZ = std::cos(currentTime * orbitSpeed) * orbitRadius;
		glBindTexture(GL_TEXTURE_2D, depthMap);

		sun->getMainObject()->setPosition(glm::vec3(sunX, sunY, sunZ));

		Game::renderScene(delta, {trees, redLight, water, skybox, terrain, sun, firecamp, plane}, lightSpaceMatrix, depthMap);

		glm::vec3 cameraOffset = glm::vec3(glm::mat4(1.0f) * glm::vec4(0.0f, 5.0f, -15.0f, 1.0f));

		glDisable(GL_DEPTH_TEST); // Ensure it draws on top of everything

		if constexpr (!CONTROL_CAMERA)
		{
			camera->setPosition(plane->getMainObject()->getPosition() + cameraOffset);
			camera->setLookAt(plane->getMainObject()->getPosition());
			dm.update(std::dynamic_pointer_cast<Controllable>(plane->getMainObject()));
		}
		else
		{
			dm.update(camera);
		}
	}

	glfwTerminate();
	return 0;
}
