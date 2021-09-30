#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct velocity
{
	uint32_t x;
	uint32_t y;
} velocity_t;

typedef struct particle
{
	bool isSolid;

} particle_t;

#endif