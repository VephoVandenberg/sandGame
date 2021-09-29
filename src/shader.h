#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <stdint.h>

void createShader(const char *vertexFile, const char *fragmentFile, uint32_t *shader);
void useShader(uint32_t shaderID);
void setUniform1i(uint32_t shaderID, const char *name, uint32_t value);
void cleanShaderSrc(char *shaderSrc);

#endif