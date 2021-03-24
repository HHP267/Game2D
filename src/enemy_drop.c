#include "simple_logger.h"
#include <SDL.h>
#include "entity.h"
#include "player.h"
#include "enemy_drop.h"
#include "shape.h"

typedef struct{
	int speed;
	float accel;
	float health, maxHealth;
}DropperStats;

static DropperStats data = { 3, 2.4, 0, 100 };

Entity *dropperSpawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("Failed to create walker");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_image("images/monsterSpike.png", 56, 56, 16);
	ent->w = 55;
	ent->h = 49;
	vector2d_copy(ent->position, position);
	vector2d_set(ent->flip, 0, 0);
	vector2d_set(ent->facing, -1, 0);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->update = dropperUpdate;

	ent->player = false;
	ent->enemy = true;
	ent->platform = false;
	ent->collect = false;
	ent->enter = false;
	ent->warp = false;
	return ent;
}

void dropperUpdate(Entity *self)
{
	if (playerUnderneath() && self->velocity.y == 0)
	{
		self->velocity.y += 3;
	}
	
	if (self->position.y >= 650)
	{
		self->velocity.y = 0;
	}
}


/**/