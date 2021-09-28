#version 330 core

layout (location = 0) in vec2 vertCoords;
layout (location = 1) in vec2 texCoords;

out vec2 outTexCoords;

void main()
{
	gl_Position = vertCords;
	outTexCoords = texCoords;
}