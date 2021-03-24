#include "simple_logger.h"
#include <SDL.h>
#include "player.h"
#include "shape.h"
#include "transition.h"

Entity *floorCreate(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("Failed to create floor");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_image("images/Grounds.png", 56, 56, 16);
	ent->w = 100;
	ent->h = 24;
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->update = NULL;

	ent->player = false;
	ent->enemy = false;
	ent->platform = true;
	ent->collect = false;
	ent->enter = false;
	ent->warp = false;
	return ent;
}
