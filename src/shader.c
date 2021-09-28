#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

void createShader(const char *vertexFile, const char *fargmentFile, uint32_t *shader)
{
	FILE *vertexPtr = fopen(vertexFile, "r");
	FILE *fragmentPtr = fopen(fargmentFile, "r");

	fseek(vertexPtr, 0, SEEK_END);
	uint32_t vertexFileLength = ftell(vertexPtr);
	fseek(vertexPtr, 0, SEEK_SET);

	char *vertexSrc = malloc(vertexFileLength);

	if (vertexSrc)
	{
		fread(vertexSrc, 1, vertexFileLength, vertexPtr);
	}
	else
	{
		printf("LOAD ERROR::VERTEX: Could not load vertex shader sourc.\n");
	}

	fseek(fragmentPtr, 0, SEEK_END);
	uint32_t fragmentFileLength = ftell(fragmentPtr);
	fseek(fragmentPtr, 0, SEEK_SET);

	char *fragmentSrc = malloc(fragmentFileLength);

	if (fragmentSrc)
	{
		fread(fragmentSrc, 1, fragmentFileLength, fragmentPtr);
	}
	else
	{
		printf("LOAD ERROR::FRAGMENT: Could not load fragment shader source.\n");
	}

	char infoLog[512];
	int success;

	uint32_t vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID, 1, &vertexSrc, 0);
	glCompileShader(vertexID);
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexID, 1024, NULL, infoLog);
		printf("ERROR::VERTEX: %s.\n", infoLog);
	}

	uint32_t fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fragmentSrc, 0);
	glCompileShader(fragmentID);
	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentID, 1024, NULL, infoLog);
		printf("ERROR::FRAGMENT: %s.\n", infoLog);
	}
}
