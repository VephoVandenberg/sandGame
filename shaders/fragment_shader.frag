#version 330 core

in vec2 outTexCoords;
out vec4 resultCoords;

uniform sampler2D texture;

void main()
{
	resultCoords = texture(texture, outTexCoords)).rgba;
}