#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdint.h>

#include "src/game.h"
#include "src/shader.h"
#include "src/particle.h"

#define SCREEN_WIDTH 950
#define SCREEN_HEIGHT 950

static void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos);

game_t game;

int main(int argc, char **argv)
{
	if (!glfwInit())
	{
		printf("Could not initialize GLFW!");
		return -1;
	}
	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sand Engine", NULL, NULL);
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		printf("Could not initialize GLEW!");
		return -1;
	}

	createShader("shaders/vertex_shader.vert", "shaders/fragment_shader.frag", &game.shaderID);
	initScreen(&game, SCREEN_WIDTH, SCREEN_HEIGHT);

	float lastFrame = 0.0f;
	float deltaTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

	
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		render(&game, deltaTime);

		glfwPollEvents();
		glfwSwapBuffers(window);

	}
	return 0;
}

static void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos)
{
	particle_t sandParticle = getSand();
	sandParticle.position.x = (uint32_t)xPos;
	sandParticle.position.y = (uint32_t)yPos;

	game.particles[game.numberOfParticles++] = sandParticle;
}