#version 330 core

in vec2 outTexCoords;
out vec3 resultCoords;

uniform sampler2D uTexture;

void main()
{
	resultCoords = texture(uTexture, outTexCoords).rgb;
}