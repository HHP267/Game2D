#include "simple_logger.h"
#include <SDL.h>
#include "enemy_walk.h"
#include "shape.h"

void walkerUpdate(Entity *self);

typedef struct{
	int speed;
	float accel;
	float health, maxHealth;
}WalkerStats;

static WalkerStats data = { 3, 2.4, 0, 100 };

Entity *walkerSpawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("Failed to create walker");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_image("images/monsterRed.png", 56, 56, 16);
	ent->w = 53;
	ent->h = 49;
	vector2d_copy(ent->position, position);
	vector2d_set(ent->flip, 0, 0);
	vector2d_set(ent->facing, -1, 0);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->update = walkerUpdate;

	ent->player = false;
	ent->enemy = true;
	ent->platform = false;
	ent->collect = false;
	ent->enter = false;
	ent->warp = false;
	return ent;
}

void walkerUpdate(Entity *self)
{

	if (self->facing.x == -1)
	{
		self->position.x += 1;

		if (self->position.x == 500)
		{
			self->flip.x = 0;
			self->facing.x = 1;
		}
	}
	else if (self->facing.x == 1)
	{
		self->position.x -= 1;

		if (self->position.x == 300)
		{
			self->flip.x = 1;
			self->facing.x = -1;
		}
	}
	
	
}

/**/