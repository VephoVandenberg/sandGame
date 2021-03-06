#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdint.h>

#include "src/game.h"
#include "src/shader.h"
#include "src/particle.h"
#include "src/math.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define BRUSH_SIZE_X 10
#define BRUSH_SIZE_Y 10

void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos);
void mouseClickCallback(GLFWwindow *window, int button, int action, int mods);
void keyboardKeysCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

game_t game;
particle_t globalParticle;
vec2f_t mouseCoords;
vec2f_t prevMouseCoords;
bool brushInUse;

int main(int argc, char **argv)
{
	if (!glfwInit())
	{
		printf("Could not initialize GLFW!");
		return -1;
	}
	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sand Engine", NULL, NULL);
	globalParticle = getSand();
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetMouseButtonCallback(window, mouseClickCallback);
	glfwSetKeyCallback(window, keyboardKeysCallback);
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

		if (brushInUse)
		{
			if (mouseCoords.x - BRUSH_SIZE_X/2 > 1.0f && mouseCoords.x + BRUSH_SIZE_X/2 < game.width &&
				mouseCoords.y - BRUSH_SIZE_Y/2 > 1.0f && mouseCoords.y + BRUSH_SIZE_Y/2 < game.height)
			{
				useBrush(&game, prevMouseCoords.x - BRUSH_SIZE_X/2, prevMouseCoords.y + BRUSH_SIZE_Y/2,
				 mouseCoords.x - BRUSH_SIZE_X/2, mouseCoords.y + BRUSH_SIZE_Y/2,
				  BRUSH_SIZE_X, BRUSH_SIZE_Y,
				   &globalParticle);
			}
		}

		render(&game, deltaTime);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	clearAll(&game);
	return 0;
}

void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos)
{
	prevMouseCoords.x = mouseCoords.x;
	prevMouseCoords.y = mouseCoords.y;

	mouseCoords.x = xPos;
	mouseCoords.y = yPos;
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

void keyboardKeysCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
		case GLFW_KEY_1:
		{
			if (action == GLFW_PRESS)
			{
				globalParticle = getSand();
			}
		}break;

		case GLFW_KEY_2:
		{
			if (action == GLFW_PRESS)
			{
				globalParticle = getWater();
			}
		}break;

		case GLFW_KEY_3:
		{
			if (action == GLFW_PRESS)
			{
				globalParticle = getSmoke();
			}
		}break;

		case GLFW_KEY_4:
		{
			if (action == GLFW_PRESS)
			{
				globalParticle = getWood();
			}
		}break;

		case GLFW_KEY_5:
		{
			if (action == GLFW_PRESS)
			{
				globalParticle = getFire();
			}
		}break;

		case GLFW_KEY_6:
		{
			if (action == GLFW_PRESS)
			{
				globalParticle = getAcid();
			}
		}break;

		case GLFW_KEY_7:
		{
			if (action == GLFW_PRESS)
			{
				globalParticle = getSalt();
			}
		}break;

		case GLFW_KEY_8:
		{
			if (action == GLFW_PRESS)
			{
				globalParticle = getDust();
			}
		}break;

		case GLFW_KEY_9:
		{
			if (action == GLFW_PRESS)
			{
				globalParticle = getDirt();
			}
		}break;
	}
}