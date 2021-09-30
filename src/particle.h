#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdbool.h>
#include <stdint.h>

typedef enum type
{
	SAND,
	WATER,
	WOOD,
	FIRE,
	SMOKE,
	ACID,
	EMPTY
} type_t;

typedef struct vec2
{
	uint32_t x;
	uint32_t y;
} vec2_t;

typedef struct particle
{
	bool isSolid;
	float lifeSpan;
	color_t color;
	vec2_t position;
	vec2_t velocity;
	type_t type;
} particle_t;

#endif