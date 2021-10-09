#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "shader.h"

#define GET_POSITION(x, y, width, height) (x >= 0 && x <= width - 1 && y >= 0 && y <= height - 1) ? y * width + x : -1
#define BROKEN_PIXEL_X 759

static color_t screenColor 	= {24,  26,  24};
static color_t sandColor   	= {255, 224, 51};
static color_t waterColor  	= {35,  137, 218};
static color_t smokeColor  	= {230, 237, 232};
static color_t woodColor 	= {133, 94,  66};
 
static vec2_t sandVelocity 	= {0,  5};
static vec2_t waterVelocity = {3,  5};
static vec2_t smokeVelocity = {0, -2};

static uint32_t gravityY = 1;

static void fillParticles(game_t *game);
static void swapParticles(particle_t *particle1, particle_t *particle2, uint32_t width, uint32_t height);

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
	updateParticles(game->particles, game->numberOfParticles, game->width, game->height);
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

void updateParticles(particle_t *particles, uint32_t numberOfParticles, uint32_t width, uint32_t height)
{
	for (uint32_t y = height - 1; y > 0; y--)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			uint32_t particlePos = GET_POSITION(x, y, width, height);
			if (particlePos == -1)
			{
				break;
			}
			switch(particles[particlePos].particleType)
			{
				case SAND:
				{
					uint32_t down;
					uint32_t leftAndDown;
					uint32_t rightAndDown;

					particles[particlePos].velocity.y += gravityY;
					for (uint32_t dVelocityY = 1; dVelocityY <= particles[particlePos].velocity.y; dVelocityY++)
					{
						down 		 = (y + dVelocityY) * width + (x);
						leftAndDown  = (y + dVelocityY) * width + (x - 1);
						rightAndDown = (y + dVelocityY) * width + (x + 1);
						
						if (y + dVelocityY >= height - 1)
						{
							down 		 = (height - 1) * width + (x);
							leftAndDown  = (height - 1) * width + (x - 1);
							rightAndDown = (height - 1) * width + (x + 1);
							break;
						}
						
						if (particles[(y + dVelocityY + 1) * width + (x)].particleType == SAND || 
							particles[(y + dVelocityY + 1) * width + (x)].particleType == WOOD)
						{
							break;
						}
						else if (particles[(y + dVelocityY + 1) * width + (x)].particleType == WATER)
						{	
							down = (y + dVelocityY - 1) * width + (x);
							break;
						}
					}

					if (!particles[particlePos].updated)
					{
						if ((particles[down].particleType == EMPTY || particles[down].particleType == WATER) && 
							 !particles[down].updated)
						{
							swapParticles(&particles[particlePos], &particles[down], width, height);
						}
						else if ((particles[leftAndDown].particleType == EMPTY || particles[leftAndDown].particleType == WATER) && 
								  y + 1 < height - 1 &&
								  x - 1 > 0)
						{
							particles[particlePos].velocity.y = sandVelocity.y;
							swapParticles(&particles[particlePos], &particles[leftAndDown], width, height);
						}
						else if ((particles[rightAndDown].particleType == EMPTY || particles[rightAndDown].particleType == WATER) && 
							     y + 1 < height - 1 &&
							     x + 1 < width - 1)
						{
							particles[particlePos].velocity.y = sandVelocity.y;
							swapParticles(&particles[particlePos], &particles[rightAndDown], width, height);
						}
					}
				}break;

				case WATER:
				{
					uint32_t down;
					uint32_t left;
					uint32_t right;
					uint32_t leftAndDown;
					uint32_t rightAndDown;

					particles[particlePos].velocity.y += gravityY;
					for (uint32_t dVelocityY = 1; dVelocityY < particles[particlePos].velocity.y; dVelocityY++)
					{
					
						down 		 = (y + dVelocityY) * width + (x);
						leftAndDown  = (y + dVelocityY) * width + (x - 1);
						rightAndDown = (y + dVelocityY) * width + (x + 1);
						
						if (y + dVelocityY >= height - 1)
						{
							down 		 = (height - 1) * width + (x);
							leftAndDown  = (height - 1) * width + (x - 1);
							rightAndDown = (height - 1) * width + (x + 1);
							break;
						}
						
						if (particles[(y + dVelocityY + 1) * width + (x)].particleType == SAND  ||
							particles[(y + dVelocityY + 1) * width + (x)].particleType == WATER ||
							particles[(y + dVelocityY + 1) * width + (x)].particleType == WOOD)
						{
							break;
						}
					}


					for (uint32_t dVelocityX = 1; dVelocityX <= particles[particlePos].velocity.x; dVelocityX+=2)
					{
						left  = particlePos - dVelocityX;
						right = particlePos + dVelocityX;

						if (x - dVelocityX <= 0)
						{
							left = (y) * width + 1;
							break;
						}

						if (x + dVelocityX >= width - 1)
						{
							right = (y) * width + (width - 1);
							break;
						}

						if (particles[left - 1].particleType == SAND  ||
							particles[left - 1].particleType == WOOD  ||
							particles[right + 1].particleType == SAND || 
							particles[right + 1].particleType == WOOD)
						{
							break;
						}
					}

					if (!particles[particlePos].updated)
					{
						if (particles[down].particleType == EMPTY && !particles[down].updated)
						{
							swapParticles(&particles[particlePos], &particles[down], width, height);
						}
						else if (particles[leftAndDown].particleType == EMPTY &&
								 y + 1 < height - 1 &&
								 x - 1 > 0)
						{
							particles[particlePos].velocity = waterVelocity;
							swapParticles(&particles[particlePos], &particles[leftAndDown], width, height);
							particles[particlePos].updated = false;
						}
						else if (particles[rightAndDown].particleType == EMPTY &&
								 y + 1 < height - 1 &&
								 x + 1 < width - 1)
						{
							particles[particlePos].velocity = waterVelocity;
							swapParticles(&particles[particlePos], &particles[rightAndDown], width, height);
							particles[particlePos].updated = false;
						}
						else if (particles[right].particleType == EMPTY && particles[down].particleType != EMPTY)
						{
							swapParticles(&particles[particlePos], &particles[right], width, height);
						}
						else if (particles[left].particleType == EMPTY && particles[down].particleType != EMPTY)
						{
							swapParticles(&particles[particlePos], &particles[left], width, height);
						}

					}
				}break;

				case SMOKE:
				{
					uint32_t up;
					uint32_t leftAndUp;
					uint32_t rightAndUp;

					for (uint32_t dVelocityY = -1; dVelocityY >= particles[particlePos].velocity.y; dVelocityY--)
					{
						up  		= (y + dVelocityY) * width + (x);
						leftAndUp 	= (y + dVelocityY) * width + (x - 1);
						rightAndUp 	= (y + dVelocityY) * width + (x + 1);

						if (y + dVelocityY <= 0)
						{
							up  		= (1) * width + (x);
							leftAndUp 	= (1) * width + (x - 1);
							rightAndUp 	= (1) * width + (x + 1);
							break;
						}

						if (particles[(y + dVelocityY - 1) * width + (x)].particleType == SMOKE)
						{
							break;
						}	
						else if (particles[(y + dVelocityY - 1) * width + (x)].particleType == WATER)
						{	
							up = (y + dVelocityY - 1) * width + (x);
							break;
						}
					}

					if (!particles[particlePos].updated)
					{
						if (particles[up].particleType != SMOKE && particles[up].particleType != WOOD && particles[up].particleType != SAND)
						{
							swapParticles(&particles[particlePos], &particles[up], width, height);
						}
						else if ((particles[leftAndUp].particleType != SMOKE && particles[up].particleType != WOOD  && particles[up].particleType != SAND) && y - 1 > 0)
						{
							swapParticles(&particles[particlePos], &particles[leftAndUp], width, height);
						}
						else if ((particles[rightAndUp].particleType != SMOKE && particles[up].particleType != WOOD && particles[up].particleType != SAND) && y - 1 > 0)
						{
							swapParticles(&particles[particlePos], &particles[rightAndUp], width, height);
						}
					}
				}break;

				case WOOD:
				{

				}break;

				case EMPTY:
				{

				}break;
			}
		}
	}
}

particle_t getSand(void)
{
	particle_t sandParticle;
	sandParticle.lifeSpan = -1.0f;
	sandParticle.particleType = SAND;
	sandParticle.velocity = sandVelocity;
	sandParticle.color = sandColor;
	return sandParticle;
}

particle_t getWater(void)
{
	particle_t waterParticle;
	waterParticle.lifeSpan = -1.0f;
	waterParticle.particleType = WATER;
	waterParticle.velocity = waterVelocity;
	waterParticle.color = waterColor;

	return waterParticle;
}

particle_t getSmoke(void)
{
	particle_t smokeParticle;
	smokeParticle.lifeSpan = -1.0f;
	smokeParticle.particleType = SMOKE;
	smokeParticle.velocity = smokeVelocity;
	smokeParticle.color = smokeColor;

	return smokeParticle;
}

particle_t getWood(void)
{
	particle_t woodParticle;
	woodParticle.lifeSpan = -1.0f;
	woodParticle.particleType = WOOD;
	woodParticle.color = woodColor;

	return woodParticle;
}

particle_t getEmpty(void)
{
	particle_t emptyParticle;
	emptyParticle.lifeSpan = -1.0f;
	emptyParticle.particleType = EMPTY;
	emptyParticle.color = screenColor;

	return emptyParticle;
}


void useBrush(game_t *game, uint32_t xPos, uint32_t yPos, uint32_t brushWidth, uint32_t brushHeight, particle_t *particle)
{
	for (uint32_t y = 0; y < brushHeight; y++)
	{
		for (uint32_t x = 0; x < brushWidth; x++)
		{
			if (game->particles[(yPos + y) * game->width + (xPos + x)].particleType == EMPTY)
			{
				game->particles[(yPos + y) * game->width + (xPos + x)] = *particle;
				game->particles[(yPos + y) * game->width + (xPos + x)].position.x = xPos + x;
				game->particles[(yPos + y) * game->width + (xPos + x)].position.y = yPos + y;
			}
		}
	}
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

static void swapParticles(particle_t *particle1, particle_t *particle2, uint32_t width, uint32_t height)
{
	if (particle1->position.x + 1 < width - 1 && particle1->position.x - 1 >= 0 &&
		particle2->position.x + 1 < width - 1 && particle2->position.x - 1 >= 0)
	{
		enum type particleType	= particle1->particleType;
		color_t color 			= particle1->color;
		vec2_t velocity 		= particle1->velocity;
		
		particle1->particleType = particle2->particleType;
		particle1->color 		= particle2->color;
		particle1->velocity 	= particle2->velocity;

		particle2->particleType = particleType;
		particle2->color 		= color;
		particle2->velocity 	= velocity;

		particle1->updated = true;
		particle2->updated = true;


	}
}
