#include "simple_logger.h"
#include <SDL.h>
#include "player.h"
#include "camera.h"
#include "shape.h"
#include "collect.h"
#include "transition.h"
#include "floor.h"

void rockUpdate(Entity *self);

Entity *rockCreate(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("Failed to create floor");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_image("images/rock_round.png");
	ent->w = 55;
	ent->h = 60;
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->_inuse = 1;
	ent->update = rockUpdate;

	ent->player = false;
	ent->enemy = false;
	ent->platform = false;
	ent->collect = false;
	ent->enter = false;
	ent->warp = false;
	ent->destruct = true;
	return ent;
}

Entity *plateCreate(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("Failed to create floor");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_image("images/keyhole.png");
	ent->w = 134;
	ent->h = 217;
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->_inuse = 1;
	ent->flip.x = -1;
	ent->update = NULL;

	ent->player = false;
	ent->enemy = false;
	ent->platform = false;
	ent->collect = false;
	ent->enter = false;
	ent->warp = false;
	ent->destruct = false;
	ent->key = true;
	return ent;
}

Entity *lockCreate(Vector2D position)
{
	Entity *ent;
	ent = entity_new();
	if (!ent)
	{
		slog("Failed to create floor");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_image("images/sand4.png");
	ent->w = 64;
	ent->h = 384;
	vector2d_copy(ent->position, position);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->_inuse = 1;
	ent->update = NULL;

	ent->player = false;
	ent->enemy = false;
	ent->platform = true;
	ent->collect = false;
	ent->enter = false;
	ent->warp = false;
	ent->destruct = false;
	ent->key = false;
	return ent;
}

Bool plateUnlock(Entity *a, Entity *b)
{
	if ((a->position.x < b->position.x + b->w) && (a->position.x + a->w > b->position.x)
		&& (a->position.y < b->position.y + b->h) && (a->position.y + a->h > b->position.y))
	{
		if ((a->destruct == true && b->key == true) || (a->key == true && b->destruct == true))
		{
			return true;
		}
	}
	return false;
}

void rockUpdate(Entity *self)
{
	destructablesContact(self);
	if (self->position.x > 1150)
	{
		self->position.x = 1150;
		self->velocity.x = 0;
	}

	if (self->position.x < 0)
	{
		self->position.x = 1;
		self->velocity.x = 0;
	}

	if (self->position.y > 650)
	{
		self->position.y = 650;	
		self->velocity.y = 0;		
	}

	if (self->position.y < 0)
	{
		self->position.y = 1;
		self->velocity.y = 0;
	}
}
