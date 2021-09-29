#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

void createShader(const char *vertexFile, const char *fargmentFile, uint32_t *shaderID)
{
	if (!shaderID)
	{
		printf("ShaderID variable has no set memory.");
	}

	FILE *vertexPtr = fopen(vertexFile, "r");
	FILE *fragmentPtr = fopen(fargmentFile, "r");

	if (!vertexPtr)
	{
		printf("FILE ERROR::VERTEX: Could not open vertex file");
	}


	if (!fragmentPtr)
	{
		printf("FILE ERROR::FRAGMENT: Could not open fragment file");
	}

	fseek(vertexPtr, 0, SEEK_END);
	uint32_t vertexFileLength = ftell(vertexPtr);
	fseek(vertexPtr, 0, SEEK_SET);

	char *vertexSrc = malloc(vertexFileLength);

	if (vertexSrc)
	{
		fread(vertexSrc, 1, vertexFileLength, vertexPtr);
		cleanShaderSrc(vertexSrc);
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
		cleanShaderSrc(fragmentSrc);
	}
	else
	{
		printf("LOAD ERROR::FRAGMENT: Could not load fragment shader source.\n");
		printf("%s", fragmentSrc);
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
		printf("ERROR::VERTEX: %s\n", infoLog);
		printf("%s", vertexSrc);
	}

	uint32_t fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fragmentSrc, 0);
	glCompileShader(fragmentID);
	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentID, 1024, NULL, infoLog);
		printf("ERROR::FRAGMENT: %s\n", infoLog);
		printf("%s", fragmentSrc);
	}

	*shaderID = glCreateProgram();
	glAttachShader(*shaderID, vertexID);
	glAttachShader(*shaderID, fragmentID);
	glLinkProgram(*shaderID);

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);

	
	fclose(vertexPtr);
	fclose(fragmentPtr);

	
	free(vertexSrc);
	free(fragmentSrc);

}

void useShader(uint32_t shaderID)
{
	glUseProgram(shaderID);
}

void setUniform1i(uint32_t shaderID, const char *name, uint32_t value)
{
	glUniform1i(glGetUniformLocation(shaderID, name), value);
}

void cleanShaderSrc(char *cleanShaderSrc)
{
	int i;
	uint8_t checked = 0;
	uint8_t detected = 0;
	for (i = 0; cleanShaderSrc[i]; i++)
	{
		if (!detected)
		{
			if (cleanShaderSrc[i] == '{')
			{
				detected = 1;
				checked += 1;
			}
		}
		else
		{
			if (cleanShaderSrc[i] == '{')
			{
				checked += 1;
			}
			else if (cleanShaderSrc[i] == '}')
			{
				checked -= 1;
			}

			if (checked == 0)
			{
				break;
			}
		}
	}
	cleanShaderSrc[++i] = '\0';
}
