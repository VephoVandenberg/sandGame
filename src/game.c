#include "game.h"
#include "shader.h"

void screenClear(game_t *game, color_t *color)
{
	for (uint32_t i = 0; i < game->width * game->height; i++)
	{
		game->data[i] = (color->r << 24) | (color->g << 16) | (color->b << 8);
	}
}

void update(float dt)
{

}