#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdint.h>

#include "src/game.h"
#include "src/shader.h"
#include "src/particle.h"

#define SCREEN_WIDTH 950
#define SCREEN_HEIGHT 950

#define BRUSH_SIZE_X 4
#define BRUSH_SIZE_Y 4

void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos);
void mouseClickCallback(GLFWwindow *window, int button, int action, int mods);

game_t game;
bool brushInUse;

int main(int argc, char **argv)
{
	if (!glfwInit())
	{
		printf("Could not initialize GLFW!");
		return -1;
	}
	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sand Engine", NULL, NULL);
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetMouseButtonCallback(window, mouseClickCallback);
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

void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (brushInUse)
	{
		uint32_t positionX = (uint32_t)xPos;
		uint32_t positionY = (uint32_t)yPos;

		if (xPos - BRUSH_SIZE_X > 1.0f && positionX + BRUSH_SIZE_X < game.width &&
			yPos - BRUSH_SIZE_Y > 1.0f && positionY + BRUSH_SIZE_Y < game.height)
		{
			particle_t sandParticle = getSand();
			useBrush(&game, positionX, positionY, BRUSH_SIZE_X, BRUSH_SIZE_Y, &sandParticle);
		}
	}
}

void mouseClickCallback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			brushInUse = true;
		}
		else if (action == GLFW_RELEASE)
		{
			brushInUse = false;
		}
	}
}