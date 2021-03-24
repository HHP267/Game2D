#include "simple_logger.h"
#include <SDL.h>
#include "entity.h"
#include "player.h"
#include "enemy_chase.h"
#include "shape.h"

typedef struct{
	int speed;
	float accel;
	float health, maxHealth;
}ChaserStats;

static ChaserStats data = { 3, 2.4, 0, 100 };

Entity *chaserSpawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("Failed to create walker");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_image("images/monsterGreen.png", 56, 56, 16);
	ent->w = 55;
	ent->h = 49;
	vector2d_copy(ent->position, position);
	vector2d_set(ent->flip, 0, 0);
	vector2d_set(ent->facing, 1, 0);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->update = chaserUpdate;

	ent->player = false;
	ent->enemy = true;
	ent->platform = false;
	ent->collect = false;
	ent->enter = false;
	ent->warp = false;
	return ent;
}

void chaserUpdate(Entity *self)
{
	if (self->facing.x == -1)
	{
		self->velocity.x = 1.5;

		if (self->position.x >= 1150)
		{
			self->flip.x = 0;
			self->facing.x = 1;
		}
	}
	else if (self->facing.x == 1)
	{
		self->position.x = -1.5;

		if (self->position.x <= 650)
		{
			self->flip.x = 1;
			self->facing.x = -1;
		}
	}
}


/**/