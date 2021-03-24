#include "simple_logger.h"
#include <SDL.h>
#include "player.h"
#include "camera.h"
#include "shape.h"
#include "collect.h"
#include "transition.h"
#include "floor.h"

Entity *spikeCreate(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("Failed to create floor");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_image("images/spikes.png");
	ent->w = 48;
	ent->h = 50;
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->update = NULL;

	ent->player = false;
	ent->enemy = true;
	ent->platform = false;
	ent->collect = false;
	ent->enter = false;
	ent->warp = false;
	return ent;
}

