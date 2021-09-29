#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdint.h>

#include "src/game.h"
#include "src/shader.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

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
	printf("%f %f\n", xPos, yPos);
}