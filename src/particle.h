#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdbool.h>
#include <stdint.h>

#include "math.h"

enum type
{
	SAND,
	WATER,
	WOOD,
	FIRE,
	SMOKE,
	ACID,
	SALT,
	DUST,
	DIRT,
	EMPTY
};

typedef struct particle
{
	bool updated;
	bool isSolid;
	float lifeSpan;
	color_t color;
	vec2_t position;
	vec2_t velocity;
	enum type particleType;
} particle_t;

#endif
