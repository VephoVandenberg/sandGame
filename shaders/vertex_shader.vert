#version 330 core

layout (location = 0) in vec2 vertCoords;
layout (location = 1) in vec2 texCoords;

out vec2 outTexCoords;

void main()
{
	gl_Position = vec4(vertCoords, 0.0f, 1.0f);
	outTexCoords = texCoords;
}