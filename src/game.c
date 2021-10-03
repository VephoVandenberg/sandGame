#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "shader.h"

static color_t screenColor = {24, 26, 24};
static color_t sandColor = {255, 224, 51};
static uint32_t velocityY = 10;

static void fillParticles(game_t *game);
static void swapParticles(particle_t *particle1, particle_t *particle2);

uint32_t colorToInt32(color_t *color);

void screenClear(game_t *game, uint32_t color)
{
	for (uint32_t i = 0; i < game->numberOfParticles; i++)
	{
		game->data[i] = color;
		game->particles[i].updated = false;
	}
}

void initScreen(game_t *game, uint32_t screenWidth, uint32_t screenHeight)
{
	if (game)
	{
		game->width 	= screenWidth;
		game->height 	= screenHeight;
		game->numberOfParticles = game->width * game->height;
		game->data 		= malloc(sizeof(uint32_t) * game->numberOfParticles);
		game->particles = malloc(sizeof(particle_t) * game->numberOfParticles);
		fillParticles(game);
		screenClear(game, colorToInt32(&screenColor));

		color_t segmentColor = {0, 255, 0};
		
		uint32_t VBO, IBO;

		float screenVertices[16] = {
			-1.0f, -1.0f,   0.0f, 1.0f,
			-1.0f,  1.0f,   0.0f, 0.0f,
			 1.0f, -1.0f, 	1.0f, 1.0f,
			 1.0f,  1.0f, 	1.0f, 0.0f
		};

		uint32_t screenIndexes[6] = {
			0, 1, 2,
			1, 2, 3
		};

		glGenVertexArrays(1, &game->screenVAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &IBO);

		glBindVertexArray(game->screenVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(screenIndexes), screenIndexes, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
		glEnableVertexAttribArray(1);

		glGenTextures(1, &game->screenTexture);
		glBindTexture(GL_TEXTURE_2D, game->screenTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, game->width, game->height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, game->data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		useShader(game->shaderID);
		setUniform1i(game->shaderID, "picture", 0);
	}
	else
	{
		printf("Game pointer is NULL.\n");
	}
}

uint32_t colorToInt32(color_t *color)
{
	return (color->r << 24) | (color->g << 16) | (color->b << 8) | 255;
}

void render(game_t *game, float dt)
{
	screenClear(game, colorToInt32(&screenColor));
	updateParticles(game);
	renderParticles(game);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, game->width, game->height, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, game->data);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void renderParticles(game_t *game)
{
	for (int i = 0; i < game->numberOfParticles; i++)
	{
		game->data[game->particles[i].position.y * game->width + game->particles[i].position.x] = colorToInt32(&game->particles[i].color);	
	}
}

void updateParticles(game_t *game)
{
	for (int i = 0; i < game->numberOfParticles; i++)
	{
		switch(game->particles[i].particleType)
		{
			case EMPTY:
			{

			}break;

			case SAND:
			{
				if (game->particles[i].position.y < game->height - 1 && !game->particles[i].updated)
				{
					uint32_t down = (game->particles[i].position.y + 1) * game->width + game->particles[i].position.x;
					uint32_t leftAndDown = (game->particles[i].position.y + 1) * game->width + game->particles[i].position.x - 1;
					uint32_t rightAndDown = (game->particles[i].position.y + 1) * game->width + game->particles[i].position.x + 1;

					if (game->particles[down].particleType == EMPTY)
					{
						swapParticles(&game->particles[i], &game->particles[down]);
					}
					else if (game->particles[leftAndDown].particleType == EMPTY)
					{
						swapParticles(&game->particles[i], &game->particles[leftAndDown]);
					}
					else if (game->particles[rightAndDown].particleType == EMPTY)
					{
						swapParticles(&game->particles[i], &game->particles[rightAndDown]);
					}
				}
			}break;
		}
	}
	
}

particle_t getSand(void)
{
	particle_t sandParticle;
	sandParticle.isSolid = false;
	sandParticle.lifeSpan = -1.0f;
	sandParticle.particleType = SAND;
	sandParticle.color = sandColor;

	return sandParticle;
}

particle_t getWater(void)
{
	particle_t waterParticle;
	waterParticle.isSolid = false;
	waterParticle.lifeSpan = -1.0f;
	waterParticle.particleType = WATER;
	color_t color = {35, 137, 218};
	waterParticle.color = color;

	return waterParticle;
}

particle_t getEmpty(void)
{
	particle_t emptyParticle;
	emptyParticle.isSolid = false;
	emptyParticle.lifeSpan = -1.0f;
	emptyParticle.particleType = EMPTY;
	emptyParticle.color = screenColor;

	return emptyParticle;
}

static void fillParticles(game_t *game)
{
	for (int y = 0; y < game->height; y++)
	{
		for (int x = 0; x < game->width; x++)
		{
			game->particles[y * game->width + x] = getEmpty();
			game->particles[y * game->width + x].position.x = x;
			game->particles[y * game->width + x].position.y = y;

		}
	}
}

static void swapParticles(particle_t *particle1, particle_t *particle2)
{
	particle_t temp;
	temp.particleType = particle1->particleType;
	temp.color = particle1->color;
	
	particle1->particleType = particle2->particleType;
	particle1->color 		= particle2->color;

	particle2->particleType = temp.particleType;
	particle2->color 		= temp.color;

	particle1->updated = true;
	particle2->updated = true;
}
