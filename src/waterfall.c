#include "simple_logger.h"
#include "entity.h"
#include "waterfall.h"

Entity *waterfallSpawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("Failed to create entity");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_all("images/waterfallCave.png", 256, 512, 8);
	vector2d_copy(ent->position, position);
	ent->frameRate = .1;
	ent->frameCount = 8;

	ent->animate = true;
	ent->player = false;
	ent->enemy = false;
	ent->platform = false;
	ent->collect = false;
	ent->enter = false;
	ent->warp = false;

	return ent;
}


/*eof*/