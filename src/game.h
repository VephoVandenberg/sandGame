#ifndef GAME_H
#define GAME_H

#include <GL\glew.h>
#include <stdint.h>

#include "color.h"
#include "math.h"
#include "particle.h"

typedef struct game
{
	uint32_t width;
	uint32_t height;
	uint32_t *data;

	uint32_t screenVAO;
	uint32_t screenTexture;

	uint32_t shaderID;

	particle_t *particles;
	uint32_t numberOfParticles;
}game_t;

void initScreen(game_t *game, uint32_t screenWidth, uint32_t screenHeight);
void screenClear(game_t *game, uint32_t color);
void render(game_t *game, float dt);

void renderParticles(game_t *game);
void updateParticles(game_t *game);

particle_t getSand(void);
particle_t getWater(void);
particle_t getEmpty(void);

#endif