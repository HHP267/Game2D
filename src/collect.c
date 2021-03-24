#include "simple_logger.h"
#include <SDL.h>
#include "player.h"
#include "camera.h"
#include "shape.h"
#include "collect.h"
#include "transition.h"
#include "floor.h"

Entity *collectCreate(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("Failed to create floor");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_image("images/Gems/PNG/Morganite Crystal01.png");
	ent->w = 64;
	ent->h = 64;
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->update = collectUpdate;

	ent->player = false;
	ent->enemy = false;
	ent->platform = false;
	ent->collect = true;
	ent->enter = false;
	return ent;
}

void collectUpdate(Entity *self)
{
	if (playerCollectContact())
	{
		entity_free(self);
	}
}
