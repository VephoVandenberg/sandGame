#ifndef GAME_H
#define GAME_H

#include <GL\glew.h>
#include <stdint.h>

typedef struct game
{
	uint32_t width;
	uint32_t height;
	uint32_t *data;
}game_t;

typedef struct color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
}color_t;

void screenClear(game_t *game, color_t *color);
void update(float dt);

#endif