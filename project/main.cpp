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

#include "game_engine/entity/camera.h"
#include "game_engine/entity/light.h"
#include "game_engine/manager/controllerManager.h"
#include "game_engine/manager/displaymanager.h"
#include "game_engine/shader.h"
#include "game_engine/waterFrameBuffer.h"

#include "game_engine/entity/skybox.h"
#include "game_engine/manager/lightManager.h"
#include "game_engine/renderer/skyboxRenderer.h"

#include <glm/gtx/string_cast.hpp>
#include "game_engine/entity/particleGenerator.h"
#include "game_engine/entity/renderableEntityMaker.h"
#include "game_engine/game.h"
#include "game_engine/manager/mainCamera.h"
#include "game_engine/prop/propMaker.h"
#include "game_engine/renderer/waterRenderer.h"
#include "game_engine/shadow.h"
#include "utils/constants.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);


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
	auto [heightMap, terrain] =
		RenderableEntityMaker::terrainFromTexture(PATH_TO_SRC "/../assets/textures/iceland_heightmap.png", PLAN_SIZE_X, PLAN_SIZE_X);

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

	// Objects
	auto water = RenderableEntityMaker::makeRenderable<Object, WaterRenderer>(
		fbos, Mesh::createPlane(PLAN_SIZE_X / 2, WATER_HEIGHT));

	auto redLight = PropMaker::makeLamp(
		glm::vec3(1.0, 15.0, 1.5), glm::vec3(1), glm::vec3(1, 1, 1),
		glm::vec4(0.1, 0.9, 1, 32), glm::vec3(0.5, 0.01, 0));

	auto plane = PropMaker::makePlane(heightMap);

    ControllerManager::get()->setPlayer(std::dynamic_pointer_cast<Player>(plane->getMainObject()));
    ControllerManager::get()->setIsPlayerControlled(DEFAULT_CAMERA_LOCKED_ON_PLAYER);

	dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);
	auto trees = PropMaker::makeTrees(heightMap);
	auto &lightManager = LightManager::get();
	float orbitRadius = PLAN_SIZE_X / 2; // Distance from the center of the scene
	float orbitSpeed = 0.1f;			 // How fast the sun moves
	float orbitHeight = 100.0f;			 // Vertical height of the sun

	auto firecamp = PropMaker::makeFirecamp(5, 0, heightMap);
	double lastTime = glfwGetTime();

	auto shadow = Shadow();

	auto game = Game();

	while (!dm.shouldClose())
	{
		const double currentTime = glfwGetTime();
		const double delta = currentTime - lastTime;
		glEnable(GL_DEPTH_TEST);
		lastTime = currentTime;

		lightManager.updateUBO();
		camera->updateUBO();

		// == SHADOWS ==
		glm::vec3 target = glm::vec3(0.0f, 50.0f, 0.0f); // Terrain center, some height
		glm::mat4 lightView = glm::lookAt(sun->getMainObject()->getPosition(), target, glm::vec3(0.0f, 1.0f, 0.0f));

		float orthoSize = PLAN_SIZE_X * 0.7f;
		glm::mat4 lightProjection = glm::ortho(-300.0f, 300.0f, -100.0f, 400.0f,
											   0.1f, 1000.0f);

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, shadow.depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

	    //TODO re-enable shadows
	    //Game::renderShadows(delta, {trees, plane, terrain}, lightSpaceMatrix, shadow.depthMap, shadow.shadowShader);

	    glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK); // Reactivate color drawing
		glReadBuffer(GL_BACK);
		dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);
	    if (water->shouldRender())
	    {
	        // == REFLECTION ==
	        glEnable(GL_CLIP_DISTANCE0);
	        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	        fbos->bindReflectionFrameBuffer();
	        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	        camera->prepareReflection(WATER_HEIGHT);
	        camera->updateUBO();
	        fbos->setClipPlane(reflectionPlane);

	        Game::renderScene(delta, {trees, skybox, terrain});
	        camera->resetCameraAfterReflection(WATER_HEIGHT);
	        camera->updateUBO();

	        // 2. Refraction
	        fbos->bindRefractionFrameBuffer();
	        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	        fbos->setClipPlane(refractionPlane); // One clean call
	        Game::renderScene(delta, {trees, terrain});

	        fbos->unbindCurrentFrameBuffer();
	        dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);
	    }

		// == NORMAL ==
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glDisable(GL_CLIP_DISTANCE0);

		float sunX = 0.0f;
		float sunY = std::sin(currentTime * orbitSpeed) * orbitRadius;
		float sunZ = std::cos(currentTime * orbitSpeed) * orbitRadius;
		glBindTexture(GL_TEXTURE_2D, shadow.depthMap);

		sun->getMainObject()->setPosition(glm::vec3(sunX, sunY, sunZ));

		Game::renderScene(delta, {trees, redLight, water, skybox, sun, firecamp, plane, terrain});

		glDisable(GL_DEPTH_TEST); // Ensure it draws on top of everything

	    dm.update();
	}

	glfwTerminate();
	return 0;
}