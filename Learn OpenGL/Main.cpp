#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "stb_image.h"
#include "ChunkManager.h"
#include <thread>

#define FMT_HEADER_ONLY

void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void update(GLFWwindow * window);
void render();
void updateFPSCounter(GLFWwindow * window);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float windowWidth = 800, windowHeight = 600;
float lastMouseX = 400, lastMouseY = 300;
float yaw = 0.0, pitch = 0.0;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

didey::chunk::ChunkManager * manager;

int main() 
{
	if (!glfwInit()) {
		std::cout << "Failed to init GLFW." << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow * window = glfwCreateWindow(windowWidth, windowHeight, "DIDEY", NULL, NULL);
	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD." << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader shader("shader.vert", "shader.frag");

	shader.use();
	
	glm::mat4 view, projection;

	manager = new didey::chunk::ChunkManager(shader.ID);
	
	while (!glfwWindowShouldClose(window)) {
		update(window);
		processInput(window);
		
		projection = glm::perspective(glm::radians(70.0f), windowWidth / windowHeight, 0.1f, 100.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void update(GLFWwindow * window) {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	updateFPSCounter(window);

	manager->queueValidChunkPositions(cameraPos, window);	
	manager->addChunkFromQueue();
}

double lastTime = glfwGetTime();
int nFrames = 0;

void updateFPSCounter(GLFWwindow * window) {
	double currentTime = glfwGetTime();
	nFrames++;
	if (currentTime - lastTime >= 1.0) {
		std::string frames = "DIDEY - FPS: " + std::to_string(nFrames);
		glfwSetWindowTitle(window, frames.c_str());
		nFrames = 0;
		lastTime = glfwGetTime();
	}
}

void render() {
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glClearColor(0.26f, 0.53f, 0.96f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	manager->renderActiveChunks(cameraPos);
}

void processInput(GLFWwindow * window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true);
	}
	
	float cameraSpeed = 20.0f * deltaTime;
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
	{
		cameraPos += cameraFront * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
	{
		cameraPos -= cameraFront * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
	{
		cameraPos.y += cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		cameraPos.y -= cameraSpeed;
	}

}

bool firstMouse = true;

void mouse_callback(GLFWwindow * window, double xpos, double ypos) 
{
	if (firstMouse) {
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastMouseX;
	float yOffset = lastMouseY - ypos;
	lastMouseX = xpos;
	lastMouseY = ypos;

	float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;
	
	if (pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// Called by GLFW on window resize, change the viewport of openGL so everything still renders properly.
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}