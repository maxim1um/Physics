#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>

#include <ctime>
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "container.h"
#include "smoothDistance.h"
#include "particle.h"
#include "checklist.h"
#include "kernel.h"
#include "collision.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Setting
GLuint SCREEN_WIDTH = 800;
GLuint SCREEN_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 1.0f, 4.0f));
GLfloat lastX = SCREEN_WIDTH / 2.0f;
GLfloat lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Lighting position
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Model position
std::vector<glm::vec3> modelPos = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

// Shader Control Bit
GLuint controlBit = 1;

GLuint containerVAO;
GLuint containerVBO;

// SPH property
const GLuint numParticles = 200;
const GLfloat smoothLength = 0.18f;
const GLfloat radius = 0.0457f;

int main(int argc, char* argv[])
{
	// GLFW initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW window creation
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Texture", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Enable face culling
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CW);

	// Build and compile shader program
	//Shader teapotShader("resources/shaders/teapot.vert", "resources/shaders/teapot.frag");
	//Shader containerShader("resources/shaders/container.vert", "resources/shaders/container.frag");
	Shader particleShader("resources/shaders/particle.vert", "resources/shaders/particle.frag");

	// Load models
	Model teapot("resources/models/teapot.obj");
	Model sphere("resources/models/sphere/sphere.obj");

	// Build container
	Container container;
	glGenVertexArrays(1, &containerVAO);
	glBindVertexArray(containerVAO);
	glGenBuffers(1, &containerVBO);
	glBindBuffer(GL_ARRAY_BUFFER, containerVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(container.vertices), container.vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create Particle simulator
	srand((unsigned)time(NULL));
	Particle* particles = new Particle[numParticles];

	for (GLuint i = 0; i != numParticles; i++)
	{
		particles[i].setID(i);
		particles[i].setPos(
			glm::vec3(0.0f + ((-500 + rand() % 1000) / 2000.0f),
				0.5f + ((-500 + rand() % 1000) / 5000.0f),
				0.0f + ((-500 + rand() % 1000) / 1000.0f)));
		particles[i].setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	//std::list<SmoothDistance> distanceList;
	glm::vec3 position[numParticles];

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Per-frame time logic
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		processInput(window);

		// Clear render buffer 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Move light source position
		//lightPos.x = 1.0f + glm::sin(glfwGetTime()) * 2.0f;
		//lightPos.y = glm::sin(glfwGetTime() / 2.0f) * 1.0f;
		//lightPos.z = -1.0f + glm::sin(glfwGetTime());

		for (GLuint i = 0; i != numParticles; i++)
		{
			// Calculate particle density
			CalculateDensity(i, particles, smoothLength, numParticles);
		}
		for (GLuint i = 0; i != numParticles; i++)
		{
			// Calculate particle pressure
			CalculatePressure(i, particles);
		}

		for (GLuint i = 0; i != numParticles; i++)
		{
			// Pressure Kernel
			PressureKernel(i, particles, smoothLength);
		}

		for (GLuint i = 0; i != numParticles; i++)
		{
			// Viscosity Kernel
			ViscosityKernel(i, particles, smoothLength);
		}

		for (GLuint i = 0; i != numParticles; i++)
		{
			// Fresh distance list
			particles[i].distanceList.clear();
		}

		for (GLuint i = 0; i != numParticles; i++)
		{
			// Get particle present position
			position[i] = particles[i].getPos();
		}

		for (GLuint i = 0; i != numParticles; i++)
		{
			// Update particle's state
			UpdateState(i, particles, deltaTime);
		}

		// Model/view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 model;
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));


		// Draw particle
		for (GLuint i = 0; i != numParticles; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, position[i]);
			model = glm::scale(model, glm::vec3(radius, radius, radius));
			// Normal Matrix
			glm::mat3 NormalMatrix = glm::transpose(glm::inverse(model));

			particleShader.use();
			particleShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
			particleShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
			particleShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
			particleShader.setFloat("material.shininess", 32.0);
			particleShader.setVec4("light.lightVector", glm::vec4(lightPos, 1.0f)); // Determine the property of light - directional or point(omni)
			particleShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
			particleShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
			particleShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
			particleShader.setVec3("viewPos", camera.Position);

			particleShader.setMat4("projection", projection);
			particleShader.setMat4("view", view);
			particleShader.setMat4("model", model);
			particleShader.setMat3("NormalMatrix", NormalMatrix);

			sphere.Draw(particleShader);
		}

		//// Enable shader before setting uniforms
		//teapotShader.use();
		//teapotShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		//teapotShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		//teapotShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		//teapotShader.setFloat("material.shininess", 32.0);
		//teapotShader.setVec4("light.lightVector", glm::vec4(lightPos, 1.0f)); // Determine the property of light - directional or point(omni)
		//teapotShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		//teapotShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		//teapotShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		//teapotShader.setVec3("viewPos", camera.Position);
		//teapotShader.setInt("controlBit", controlBit);

		//// Pass projection, view and model matrix to phong shader
		//teapotShader.setMat4("projection", projection);
		//teapotShader.setMat4("view", view);
		//teapotShader.setMat4("model", model);
		//teapotShader.setMat3("NormalMatrix", NormalMatrix);

		//// Render the loaded model
		//teapot.Draw(teapotShader);

		//model = glm::mat4();
		//containerShader.use();
		//containerShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		//containerShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		//containerShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		//containerShader.setFloat("material.shininess", 32.0);
		//containerShader.setVec4("light.lightVector", glm::vec4(lightPos, 1.0f));
		//containerShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		//containerShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		//containerShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		//containerShader.setVec3("viewPos", camera.Position);

		//containerShader.setMat4("projection", projection);
		//containerShader.setMat4("view", view);
		//containerShader.setMat4("model", model);
		//containerShader.setMat3("NormalMatrix", NormalMatrix);
		//glBindVertexArray(containerVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);



		//particleShader.use();
		//particleShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		//particleShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		//particleShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		//particleShader.setFloat("material.shininess", 32.0);
		//particleShader.setVec4("light.lightVector", glm::vec4(lightPos, 1.0f)); // Determine the property of light - directional or point(omni)
		//particleShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		//particleShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		//particleShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		//particleShader.setVec3("viewPos", camera.Position);

		//particleShader.setMat4("projection", projection);
		//particleShader.setMat4("view", view);
		//particleShader.setMat4("model", model);
		//particleShader.setMat3("NormalMatrix", NormalMatrix);

		//sphere.Draw(particleShader);

		// Swap buff
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyborad(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyborad(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyborad(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyborad(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		controlBit = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		controlBit = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		controlBit = 3;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		controlBit = 4;
	}
}
