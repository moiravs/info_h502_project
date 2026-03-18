#include <iostream>

// include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include "camera.h"
#include "object.h"

#include <chrono>
#include <thread>

const int width = 700;
const int height = 500;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#ifndef NDEBUG
// source: https://learnopengl.com/In-Practice/Debugging
void APIENTRY glDebugOutput(GLenum source,
							GLenum type,
							unsigned int id,
							GLenum severity,
							GLsizei length,
							const char *message,
							const void *userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;
	if (id == 1282)
		return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		std::cout << "Source: API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		std::cout << "Source: Window System";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		std::cout << "Source: Shader Compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		std::cout << "Source: Third Party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		std::cout << "Source: Application";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		std::cout << "Source: Other";
		break;
	}
	std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "Type: Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "Type: Deprecated Behaviour";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "Type: Undefined Behaviour";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "Type: Portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "Type: Performance";
		break;
	case GL_DEBUG_TYPE_MARKER:
		std::cout << "Type: Marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		std::cout << "Type: Push Group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		std::cout << "Type: Pop Group";
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "Type: Other";
		break;
	}
	std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "Severity: high";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "Severity: medium";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "Severity: low";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cout << "Severity: notification";
		break;
	}
}
#endif

Camera camera(glm::vec3(0.0, 0.0, 0.1));

void processInput(GLFWwindow *window)
{
	// Use the cameras class to change the parameters of the camera
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(LEFT, 0.1);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(RIGHT, 0.1);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(FORWARD, 0.1);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(BACKWARD, 0.1);

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(1, 0.0, 1);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(-1, 0.0, 1);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(0.0, 1.0, 1);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(0.0, -1.0, 1);
}

int main(int argc, char *argv[])
{

	// Create the OpenGL context
	if (!glfwInit())
	{
		throw std::runtime_error("Failed to initialise GLFW \n");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifndef NDEBUG
	// A1. Request a debug context with glfw
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	// Create the window
	GLFWwindow *window = glfwCreateWindow(width, height, "Labo 04", nullptr, nullptr);
	if (window == NULL)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window\n");
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}

#ifndef NDEBUG
	// 2. Tell OpenGL to enable debug output and to call the glDebugOutput function(defined above in this code)\n
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif

	// INIT shaders
	Shader bunny(PATH_TO_SRC "/../assets/shaders/bunny.vert", PATH_TO_SRC "/../assets/shaders/bunny.frag");
	Shader shader(PATH_TO_SRC "/../assets/shaders/FBO.vert", PATH_TO_SRC "/../assets/shaders/FBO.frag");
	Shader plane_sh(PATH_TO_SRC "/../assets/shaders/plane.vert", PATH_TO_SRC "/../assets/shaders/tex.frag");

	// INIT models
	Object cube(PATH_TO_OBJECTS "/bunny_small.obj");

	// CREATE models
	cube.makeObject(bunny);

	const float planeData[30] = {
		// vertices		  // texture coords
		-1.0, -1.0, 0.0, 0.0, 0.0,
		1.0, -1.0, 0.0, 1.0, 0.0,
		-1.0, 1.0, 0.0, 0.0, 1.0,
		1.0, 1.0, 0.0, 1.0, 1.0,
		-1.0, 1.0, 0.0, 0.0, 1.0,
		1.0, -1.0, 0.0, 1.0, 0.0};

	// Create the vertex buffer object and the vertex array object
	GLuint VBO, VAO;
	// generate the buffer and the vertex array
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// define VBO and VAO as active buffer and active vertex array
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeData), planeData, GL_STATIC_DRAW);

	auto attribute = glGetAttribLocation(shader.ID, "position");
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, 3, GL_FLOAT, false, 5 * sizeof(float), (void *)0);

	// desactive the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLuint VBO2, VAO2;
	// generate the buffer and the vertex array
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);

	// define VBO and VAO as active buffer and active vertex array
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeData), planeData, GL_STATIC_DRAW);

	auto att_pos = glGetAttribLocation(plane_sh.ID, "position");
	glEnableVertexAttribArray(att_pos);
	glVertexAttribPointer(att_pos, 3, GL_FLOAT, false, 5 * sizeof(float), (void *)0);

	auto att_tex = glGetAttribLocation(plane_sh.ID, "texcoord");
	glEnableVertexAttribArray(att_tex);
	glVertexAttribPointer(att_tex, 2, GL_FLOAT, false, 5 * sizeof(float), (void *)(3 * sizeof(float)));

	// desactive the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int framebuffer_width, framebuffer_height;
	int window_width, window_height;
	double mouse_x, mouse_y;
	float scale_x, scale_y;
	glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);

	glViewport(0, 0, framebuffer_width, framebuffer_height);
	// Create the frame buffer
	GLuint FrameBufferEffect = 0;
	glGenFramebuffers(1, &FrameBufferEffect);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferEffect);

	// Create the texture that contains the rgb output of the shader
	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);

	// Bind the new texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	// give an empty image to opengl
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, framebuffer_width, framebuffer_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	GLuint depthRenderBuffer;
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, framebuffer_width, framebuffer_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.5, 0.5, -1.0));
	model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));

	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 perspective = camera.GetProjectionMatrix();

	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		view = camera.GetViewMatrix();

		glfwPollEvents();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// switch on the frame buffer for the texture
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferEffect);

		shader.use();

		glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
		shader.setVector2f("iResolution", framebuffer_width, framebuffer_height);

		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		glfwGetWindowSize(window, &window_width, &window_height);
		scale_x = framebuffer_width / window_width;
		scale_y = framebuffer_height / window_height;

		shader.setVector2f("iMouse", (int)(mouse_x * scale_x), (int)(mouse_y * scale_y));

		shader.setInteger("renderedTexture", 1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Go  to the "regular" framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindVertexArray(VAO2);

		plane_sh.use();
		plane_sh.setInteger("renderedTexture", 0);
		// Use the shader Class to send the uniform

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		bunny.use();

		bunny.setMatrix4("M", model);
		bunny.setMatrix4("V", view);
		bunny.setMatrix4("P", perspective);
		cube.draw();
		glfwSwapBuffers(window);
	}

	// clean up ressource
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
