#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include "src/game.h"
#include "src/shader.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

static void cursorPositionCallback(GLFWwindow *window, float xPos, float yPos);

game_t game;

int main(int argc, char **argv)
{
	game.width = SCREEN_WIDTH;
	game.height = SCREEN_HEIGHT;

	if (!glfwInit())
	{
		printf("Could not initialize GLFW!");
		return -1;
	}

	GLFWwindow *window = glfwCreateWindow(game.width, game.height, "Sand Engine", NULL, NULL);
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		printf("Could not initialize GLEW!");
		return -1;
	}

	int a = 5;
	createShader("shaders/vertex_shader.vert", "shaders/fragment_shader.frag", &a);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

static void cursorPositionCallback(GLFWwindow *window, float xPos, float yPos)
{
	printf("%f %f\n", xPos, yPos);
}