#include "simple_logger.h"
#include <SDL.h>
#include "player.h"
#include "camera.h"
#include "shape.h"
#include "collect.h"
#include "transition.h"
#include "floor.h"

void playerUpdate(Entity *self);

typedef struct{
	int speed;
	float accel;
	float health, maxHealth;
}PlayerStats;

static PlayerStats data = { 3, 2.4, 0, 100 };

Vector2D location;
int score = 0;

Entity *playerSpawn(Vector2D position)
{
	Entity *ent; 
	ent = entity_new();
	if (!ent)
	{
		slog("Failed to create player");
		return NULL;
	}
	ent->sprite = gf2d_sprite_load_image("images/mole.png", 56, 56, 16);
	ent->w = 53;
	ent->h = 73;
	vector2d_copy(ent->position, position);
	vector2d_set(ent->flip, 0, 0);
	vector2d_set(ent->facing, 1, 0);
	vector2d_set(ent->velocity, 0, 0);
	vector2d_set(ent->acceleration, 0, 0);
	ent->frameRate = 0.1;
	ent->frameCount = 16;
	ent->update = playerUpdate;

	ent->player = true;
	ent->enemy = false;
	ent->platform = false;
	ent->collect = false;
	ent->enter = false;
	ent->warp = false;

	return ent;
}

void playerBoundaries(Entity *self)
{
	if (self->position.x > 1150)
	{
		self->position.x = 1150;
	}

	if (self->position.x < 0)
	{
		self->position.x = 1;
	}

	if (self->position.y > 650)
	{
		if (self->worldState == 0)
		{
			self->position.y = 650;
		}
		else if (self->worldState == 1)
		{
			playerDie(self);
		}
	}

	if (self->position.y < 0)
	{
		self->position.y = 1;
	}
}

void playerFindPosition(Entity *self)
{
	location.x = self->position.x;
	location.y = self->position.y;
}

Vector2D returnPlayerPosition()
{
	return location;
}

void playerDie(Entity *self)
{
	FILE *fp;
	fp = fopen("scores.txt", "w+");
	fprintf(fp, "Final Score: %f \n", score);
	fclose(fp);
	entity_free(self);
}

int returnScore()
{
	return score;
}

void updateScore()
{
	score += 10;
	slog("Score: %f", score);
}

void playerUpdate(Entity *self)
{
	const Uint8 *movement;
	movement = SDL_GetKeyboardState(NULL);

	playerBoundaries(self);
	playerWarp();
	if (playerEnemyContact())
	{
		playerDie(self);
	}
	if (entityGrounded())
	{
		self->velocity.x = 0;
	}
	if (playerCollectContact())
	{
		score += 10;
		slog("Score %d", returnScore());
	}


	if (getWorldState() == 0) self->worldState = 0;
	else if (getWorldState() == 1) self->worldState = 1;



	if (self->worldState == 0)
	{
		if (movement[SDL_SCANCODE_UP])self->position.y -= 2;
		if (movement[SDL_SCANCODE_DOWN])self->position.y += 2;
		if (movement[SDL_SCANCODE_LEFT])
		{
			self->position.x -= 2;
			self->flip.x = 0;
			self->facing.x = 1;
		}
		if (movement[SDL_SCANCODE_RIGHT])
		{
			self->position.x += 2;
			self->flip.x = 1;
			self->facing.x = -1;
		}
	}
	else if (self->worldState == 1)
	{
		entityGravity(self);

		if (movement[SDL_SCANCODE_LEFT])
		{
			self->position.x -= 2;
			self->flip.x = 0;
			self->facing.x = 1;
		}
		if (movement[SDL_SCANCODE_RIGHT])
		{
			self->position.x += 2;
			self->flip.x = 1;
			self->facing.x = -1;
		}
		if (movement[SDL_SCANCODE_UP])
		{
			if (entityGrounded())
			{
				self->velocity.y -= 15.5;
			}	
		}
		if (entityGrounded())
		{
			if (movement[SDL_SCANCODE_E])
			{
				self->position.y += 2;
			}
		}
		if (movement[SDL_SCANCODE_SPACE])
		{
			if (!entityGrounded())
			{
				if (self->facing.x == 1)
				{
					self->velocity.x -= .5;
				}
				else if (self->facing.x == -1)
				{
					self->velocity.x += .5;
				}
			}
		}
	}
} 


/**/
