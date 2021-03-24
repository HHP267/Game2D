#include "simple_logger.h"
#include <SDL.h>
#include "entity.h"
#include "enemy_climb.h"
#include "shape.h"

typedef struct{
	int speed;
	float accel;
	float health, maxHealth;
}ClimberStats;

static ClimberStats data = { 3, 2.4, 0, 100 };

Entity *climberSpawn(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("Failed to create walker");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_image("images/monsterOrange.png", 56, 56, 16);
	ent->w = 53;
	ent->h = 49;
	vector2d_copy(ent->position, position);
	vector2d_set(ent->flip, 0, 0);
	vector2d_set(ent->facing, -1, 0);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->update = climberUpdate;

	ent->player = false;
	ent->enemy = true;
	ent->platform = false;
	ent->collect = false;
	ent->enter = false;
	ent->warp = false;
	return ent;
}

void climberUpdate(Entity *self)
{
	if (self->flip.x == 0)
	{
		self->position.y += 1;
	}
	else if (self->flip.x == 1)
	{
		self->position.y -= 1;
	}

	if (self->position.y == 120)
	{
		self->flip.x = 1;
	}
	else if (self->position.y == 0)
	{
		self->flip.x = 0;
	}
}

/**/