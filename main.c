#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

static void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos);

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
		printf("Could not initialize GLEW");
		return -1;
	}

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.5f, 0.3f, 0.4f, 1.0f);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

static void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos)
{
	printf("%f %f\n", xPos, yPos);
}