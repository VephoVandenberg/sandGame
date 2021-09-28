#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <stdint.h>

void createShader(const char *vertexFile, const char *fragmentFile, uint32_t *shader);
void bindShader(uint32_t shaderID);

#endif