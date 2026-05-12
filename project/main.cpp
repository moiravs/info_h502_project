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
#include "game_engine/shader/shader.h"

#include "game_engine/entity/skybox.h"
#include "game_engine/manager/lightManager.h"
#include "game_engine/renderer/skyboxRenderer.h"

#include <glm/gtx/string_cast.hpp>
#include <map>
#include "game_engine/depth/depthMapFrameBuffer.h"
#include "game_engine/entity/light/directionalLight.h"
#include "game_engine/entity/particleGenerator.h"
#include "game_engine/entity/renderableEntityMaker.h"
#include "game_engine/entity/text.h"
#include "game_engine/framebuffer/geometryFrameBuffer.h"
#include "game_engine/framebuffer/lightFrameBuffer.h"
#include "game_engine/framebuffer/reflectionFrameBuffer.h"
#include "game_engine/framebuffer/refractionFrameBuffer.h"
#include "game_engine/game.h"
#include "game_engine/manager/mainCamera.h"
#include "game_engine/prop/propMaker.h"
#include "game_engine/renderer/waterRenderer.h"
#include "game_engine/renderer/spriteRenderer.h"
#include "game_engine/shader/lightShader.h"
#include "utils/constants.h"
#include "utils/textureViewer.h"
#include <map>
#include "game_engine/entity/text.h"
#include "SFML/Audio.hpp"

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

	MainCamera::init(dm.getWidth(), dm.getHeight());

	auto camera = MainCamera::get();

	Shader shader(PATH_TO_SRC "/../assets/shaders/text.vert", PATH_TO_SRC "/../assets/shaders/text.frag");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(dm.getWidth()), 0.0f, static_cast<float>(dm.getHeight()));
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// find path to font
	std::string font_name = PATH_TO_SRC "/../assets/fonts/gabriele_ribbon_fg_6918761(1)/gabriele-br.ttf";
	if (font_name.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		return -1;
	}

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// the sun must be created first.
	auto [sun, sunLight] = PropMaker::makeSun(glm::vec3(.0, 100.0, 1.5), glm::vec3(10, 10, 10),
											  glm::vec3(1, 1, 1));

	// Terrain
	auto [heightMap, terrain] =
		PropMaker::terrainFromTexture(PATH_TO_SRC "/../assets/textures/iceland_heightmap.png", PLAN_SIZE_X, PLAN_SIZE_X);

	// Skybox
	auto skybox = RenderableEntityMaker::makeRenderable<Skybox, SkyboxRenderer>(
		"skybox", std::vector<std::string>{PATH_TO_SRC "/../assets/textures/cubemaps/skybox/right.jpg",
										   PATH_TO_SRC "/../assets/textures/cubemaps/skybox/left.jpg",
										   PATH_TO_SRC "/../assets/textures/cubemaps/skybox/top.jpg",
										   PATH_TO_SRC "/../assets/textures/cubemaps/skybox/bottom.jpg",
										   PATH_TO_SRC "/../assets/textures/cubemaps/skybox/front.jpg",
										   PATH_TO_SRC "/../assets/textures/cubemaps/skybox/back.jpg"});

	// FBOs
	auto geometryFBO = std::make_shared<GeometryFrameBuffer>(dm.getWidth(), dm.getHeight());
	geometryFBO->createTextures();
	auto waterReflectionFBO = std::make_shared<ReflectionFrameBuffer>();
	waterReflectionFBO->createTextures();
	auto waterRefractionFBO = std::make_shared<RefractionFrameBuffer>();
	waterRefractionFBO->createTextures();
	auto shadowFBO = std::make_shared<DepthMapFrameBuffer>(DEPTH_WIDTH, DEPTH_HEIGHT, sunLight);
	shadowFBO->createTextures();
	auto lightFBO = std::make_shared<LightFrameBuffer>(dm.getWidth(), dm.getHeight(), geometryFBO->getDepthTex());
	lightFBO->createTextures();

	auto lightShader = std::make_shared<LightShader>();

	auto reflectionPlane = glm::vec4(0, 1, 0, WATER_HEIGHT);
	auto refractionPlane = glm::vec4(0, -1, 0, WATER_HEIGHT);

	// Objects
	auto water = RenderableEntityMaker::makeRenderable<Object, WaterRenderer>(
		Mesh::createPlane(PLAN_SIZE_X / 2, WATER_HEIGHT));

	auto plane = PropMaker::makePlane(heightMap);

	ControllerManager::get()->setPlayer(std::dynamic_pointer_cast<Player>(plane->getMainObject()));
	ControllerManager::get()->setIsPlayerControlled(DEFAULT_CAMERA_LOCKED_ON_PLAYER);

	dm.resizeViewport(dm.getWidth(), dm.getHeight());
	auto trees = PropMaker::makeTrees(heightMap);
	auto flowerField = PropMaker::makeFlowers(heightMap);
	auto &lightManager = LightManager::get();
	float orbitRadius = PLAN_SIZE_X / 2; // Distance from the center of the scene
	float orbitSpeed = 0.1f;			 // How fast the sun moves
	float orbitHeight = 100.0f;			 // Vertical height of the sun

	auto [firecamp, firecampParticles] = PropMaker::makeFirecamp(5, 0, heightMap);
	double lastTime = glfwGetTime();

	auto textureViewer = TextureViewer();
	auto rings = PropMaker::makeRings(heightMap);

	auto game = Game();
	auto spriteRenderer = new SpriteRenderer("sprite");
	Texture spriteTexture(PATH_TO_SRC "/../assets/textures/avion.png");

	auto text = Text();
	text.loadCharactersFromBitmap(ft, font_name);

	while (!dm.shouldClose())
	{
		const double currentTime = glfwGetTime();
		const double delta = currentTime - lastTime;
		if (dm.currentState == GameState::PLAYING)
		{

			glEnable(GL_DEPTH_TEST);
			lastTime = currentTime;

			Game::update(delta, {trees, water, skybox, sun, firecamp, plane, terrain, rings});

			float sunX = 0.0f;
			float sunY = std::sin(currentTime * orbitSpeed) * orbitRadius;
			float sunZ = std::cos(currentTime * orbitSpeed) * orbitRadius;

			sun->setPosition(glm::vec3(sunX, sunY, sunZ));

			lightManager.updateUBO();
			camera->updateUBO();

			// == SHADOWS ==
			shadowFBO->begin();

			Game::renderSceneWithShader({trees, terrain}, shadowFBO->getShader());

			shadowFBO->end();

			// == REFLECTION ==
			waterReflectionFBO->begin();
			waterReflectionFBO->setClipPlane(reflectionPlane);

			Game::renderScene({trees, skybox, terrain});

			waterReflectionFBO->end();

			// == REFRACTION ==

			waterRefractionFBO->begin();
			waterReflectionFBO->setClipPlane(refractionPlane);

			Game::renderScene({terrain, skybox});

			waterRefractionFBO->end();

			// == GEOMETRY ==

			geometryFBO->begin();

			Game::renderScene({trees, skybox, firecamp, plane, terrain, rings, water});

			geometryFBO->end();

			lightFBO->begin();

			geometryFBO->bindTextures();
			shadowFBO->bindTextures();

			lightShader->render();

			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);

			waterReflectionFBO->bindTextures();
			waterRefractionFBO->bindTextures();

			Game::renderScene({sun, firecampParticles});

			glDepthMask(GL_TRUE);
			lightFBO->end();

			dm.resizeViewport(dm.getWidth(), dm.getHeight());
			textureViewer.render(lightFBO->getTexture(), false);

			game.checkIfPlaneInRing(plane, rings, heightMap);

			glDisable(GL_DEPTH_TEST); // Ensure it draws on top of everything

			Game::renderText(text, shader, std::to_string(game.numberOfRings), 25.0f, 25.0f, 5.0f, glm::vec3(0.5, 0.8f, 0.2f));

			glEnable(GL_DEPTH_TEST); // Ensure it draws on top of everything
		}
		else if (dm.currentState == GameState::MENU)
		{
			glDisable(GL_DEPTH_TEST); // Ensure it draws on top of everything

			// --- MENU RENDERING ---
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Dark background for menu

			float titleWidth = text.calculateStringWidth("FLYING SIMULATOR", 2.0f);
			float playWidth = text.calculateStringWidth("PLAY", 1.0f);

			Game::renderText(text, shader, "FLYING SIMULATOR",
							 (dm.getWidth() - titleWidth) / 2.0f,
							 dm.getHeight() / 2.0f + 400.0f,
							 2.0f, glm::vec3(1, 1, 1));

			Game::renderText(text, shader, "PLAY",
							 (dm.getWidth() - playWidth) / 2.0f,
							 dm.getHeight() / 2.0f,
							 1.0f, glm::vec3(1, 1, 1));

			spriteRenderer->render(spriteTexture, glm::vec2(500.0f, 500.0f), glm::vec2(500.0f, 397.0f), 0.0f);
		}
		dm.update();
	}

	glfwTerminate();
	return 0;
}
