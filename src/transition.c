#include "simple_logger.h"
#include <SDL.h>
#include "transition.h"
#include "entity.h"
#include "player.h"
#include "shape.h"
#include "level.h"

static int worldState;
Entity *transitionLocationSpawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("Failed to create opening");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_image("images/tilesetCaveOpening.png", 56, 56, 16);
	ent->w = 32;
	ent->h = 32;
	vector2d_copy(ent->position, position);
	vector2d_set(ent->flip, 0, 0);
	vector2d_set(ent->facing, 1, 0);
	ent->worldState = 0;
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->update = NULL;
	worldState = ent->worldState;

	ent->player = false;
	ent->enemy = false;
	ent->platform = false;
	ent->collect = false;
	ent->enter = true;
	ent->warp = false;

	return ent;
}

void transitionUpdate(Entity *self, Entity *other);

int getWorldState()
{
	return worldState;
}

void changeWorldState(int levelType)
{
	if (levelType == 0)
	{
		worldState = 1;
	}
	else if (levelType == 1)
	{
		worldState = 0;
	}
}