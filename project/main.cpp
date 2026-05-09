#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <freetype/ftcolor.h>
#include "utils/utils.h"

#define STB_IMAGE_IMPLEMENTATION
// the include below must NOT be removed
#include <stb_image.h>

#include <vector>

#include "game_engine/entity/camera.h"
#include "game_engine/entity/light/light.h"
#include "game_engine/manager/controllerManager.h"
#include "game_engine/manager/displaymanager.h"
#include "game_engine/shader.h"
#include "game_engine/waterFrameBuffer.h"

#include "game_engine/entity/skybox.h"
#include "game_engine/manager/lightManager.h"
#include "game_engine/renderer/skyboxRenderer.h"

#include <glm/gtx/string_cast.hpp>
#include <map>
#include "game_engine/depth/depthMap.h"
#include "game_engine/entity/light/directionalLight.h"
#include "game_engine/entity/particleGenerator.h"
#include "game_engine/entity/renderableEntityMaker.h"
#include "game_engine/entity/text.h"
#include "game_engine/framebuffer/geometryFrameBuffer.h"
#include "game_engine/framebuffer/reflectionFrameBuffer.h"
#include "game_engine/game.h"
#include "game_engine/manager/mainCamera.h"
#include "game_engine/prop/propMaker.h"
#include "game_engine/renderer/waterRenderer.h"
#include "utils/constants.h"
#include "utils/textureViewer.h"

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

	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	Shader shader(PATH_TO_SRC "/../assets/shaders/text.vert", PATH_TO_SRC "/../assets/shaders/text.frag");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// find path to font
	std::string font_name = PATH_TO_SRC "/../assets/fonts/gabriele_ribbon_fg_6918761(1)/gabriele-br.ttf";
	if (font_name.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		return -1;
	}

    // FBOs
    auto geometryFBO = std::make_shared<GeometryFrameBuffer>(SCR_WIDTH, SCR_HEIGHT);
    geometryFBO->createTextures();
    auto waterReflectionFBO = std::make_shared<ReflectionFrameBuffer>();
    waterReflectionFBO->createTextures();

	auto [sun, sunLight] = PropMaker::makeSun(glm::vec3(.0, 100.0, 1.5), glm::vec3(10, 10, 10),
	    glm::vec3(1, 1, 1));

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
	auto flowerField = PropMaker::makeFlowers(heightMap);
	auto &lightManager = LightManager::get();
	float orbitRadius = PLAN_SIZE_X / 2; // Distance from the center of the scene
	float orbitSpeed = 0.1f;			 // How fast the sun moves
	float orbitHeight = 100.0f;			 // Vertical height of the sun

	auto firecamp = PropMaker::makeFirecamp(5, 0, heightMap);
	double lastTime = glfwGetTime();

	auto shadow = std::make_shared<DepthMap>(sunLight);

    auto textureViewer = TextureViewer();
	auto rings = PropMaker::makeRings(heightMap);

	auto game = Game();

	auto text = Text();
	text.loadCharactersFromBitmap(ft, font_name);

	while (!dm.shouldClose())
	{
		const double currentTime = glfwGetTime();
		const double delta = currentTime - lastTime;
		lastTime = currentTime;

		lightManager.updateUBO();
		camera->updateUBO();

	    Game::renderShadows({trees, plane, terrain}, shadow);

		dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);
		// == REFLECTION ==
        waterReflectionFBO->begin();
		waterReflectionFBO->setClipPlane(reflectionPlane);

		Game::renderScene(delta, {trees, skybox, terrain});

		waterReflectionFBO->end();

	    //dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);
	    //textureViewer.render(waterReflectionFBO->getTexture(), false);

	    //dm.update();
	    //continue;

		// 2. Refraction
		fbos->bindRefractionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		waterReflectionFBO->setClipPlane(refractionPlane); // One clean call
		Game::renderScene(delta, {trees, terrain});

		fbos->unbindCurrentFrameBuffer();
		dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);

		// == NORMAL ==
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glDisable(GL_CLIP_DISTANCE0);

		 float sunX = 0.0f;
		 float sunY = std::sin(currentTime * orbitSpeed) * orbitRadius;
		 float sunZ = std::cos(currentTime * orbitSpeed) * orbitRadius;

		sun->getMainObject()->setPosition(glm::vec3(sunX, sunY, sunZ));

	    geometryFBO->begin();

	    waterReflectionFBO->bindTextures();

		Game::renderScene(delta, {trees, redLight, water, skybox, sun, firecamp, plane, terrain, rings});

	    geometryFBO->end();

	    dm.resizeViewport(SCR_WIDTH, SCR_HEIGHT);
        textureViewer.render(geometryFBO->getColorTex(), false);

	    dm.update();
	    continue;

		Game::checkTerrainCollision(plane->getMainObject(), heightMap);

		glDisable(GL_DEPTH_TEST); // Ensure it draws on top of everything

		Game::renderText(text, shader, "VIONVION", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

		dm.update();
	}

	glfwTerminate();
	return 0;
}
