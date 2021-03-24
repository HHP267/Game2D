#include "simple_logger.h"
#include <SDL.h>
#include "entity.h"
#include "player.h"
#include "enemy_rise.h"
#include "shape.h"

Entity *riserSpawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("Failed to create walker");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_image("images/monsterBlue.png", 56, 56, 16);
	ent->w = 53;
	ent->h = 49;
	vector2d_copy(ent->position, position);
	vector2d_set(ent->flip, 0, 0);
	vector2d_set(ent->facing, 1, 0);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->update = riserUpdate;

	ent->player = false;
	ent->enemy = true;
	ent->platform = false;
	ent->collect = false;
	ent->enter = false;
	ent->warp = false;
	return ent;
}

void riserUpdate(Entity *self)
{
	if (self->position.y >= 650)
	{
		self->position.y -= 2;
	}
	
}


/**/