#include "entity.h"
#include "gfc_vector.h"
#include "simple_logger.h"
#include "gfc_types.h"
#include "player.h"
#include "shape.h"
#include "camera.h"

typedef struct
{
	Entity *entity_list;
	Uint32 max_entities;
}EntityManager;

static EntityManager entityManager = { 0 };

void entity_manager_init(Uint32 max_entities)
{
	if (max_entities == 0)
	{
		slog("Cannot Allocate 0 Entities!");
		return;
	}
	if (entityManager.entity_list != NULL)
	{
		entity_manager_free();
	}
	entityManager.entity_list = (Entity *)gfc_allocate_array(sizeof(Entity), max_entities);
	if (entityManager.entity_list == NULL)
	{
		slog("Failed to allocate Entity List");
		return;
	}
	entityManager.max_entities = max_entities;
	atexit(entity_manager_free);
}

void entity_manager_free()
{
	if (entityManager.entity_list != NULL)
	{
		free(entityManager.entity_list);
	}
	memset(&entityManager, 0, sizeof (EntityManager));
}

void entity_update(Entity *self)
{
	if (!self)return;
	//GENERIC UPDATE CODE
	//IF A CUSTOM UPDATE IS NEEDED, DO NOW
	vector2d_add(self->position, self->position, self->velocity);

	self->frame += self->frameRate;
	if (self->frame >= self->frameCount)
	{
		self->frame = 0;
	}
	if (self->think)
	{
		self->think(self);
	}
	if (self->update)
	{
		self->update(self);
	}
}

Entity entity_manager_return_entity(int i)
{
	return entityManager.entity_list[i];
}

Bool entity_manager_collide(Entity a, Entity b)
{
	if (entityManager.entity_list == NULL)
	{
		slog("entity does not exist");
		return;
	}
	if ((a.position.x < b.position.x + b.w) && (a.position.x + a.w > b.position.x) 
		&& (a.position.y < b.position.y + b.h) && (a.position.y + a.h > b.position.y))
	{
		//slog("collide");
		return true;
	}
	return false;
}

Bool entityGrounded()
{
	int i;
	for (i = 1; i < entityManager.max_entities; i++)
	{
		if (entity_manager_collide(entityManager.entity_list[0], entityManager.entity_list[i]))
		{
			if (entityManager.entity_list[i].platform)
			{
				return true;
			}
		}
	}
	return false;
}

Bool enemyGrounded()
{
	int i;
	for (i = 1; i < entityManager.max_entities; i++)
	{
		if (entity_manager_collide(entityManager.entity_list[2], entityManager.entity_list[i]))
		{
			if (entityManager.entity_list[i].platform)
			{
				return true;
			}
		}
	}
	return false;
}

Bool playerUnderneath()
{
	int i;
	for (i = 1; i < entityManager.max_entities; i++)
	{
		if ((entityManager.entity_list[0].position.x < entityManager.entity_list[i].position.x + entityManager.entity_list[i].w) 
			&& (entityManager.entity_list[0].position.x + entityManager.entity_list[0].w > entityManager.entity_list[i].position.x))
		{
			if (entityManager.entity_list[i].enemy)
			{
				return true;
			}
		}
	}
	return false;
}

Bool playerHorizontal()
{
	int i;
	for (i = 1; i < entityManager.max_entities; i++)
	{
		if (entityManager.entity_list[0].position.y < entityManager.entity_list[i].position.y + entityManager.entity_list[i].h
			&& entityManager.entity_list[0].position.y + entityManager.entity_list[0].h > entityManager.entity_list[i].position.y)
		{
			if (entityManager.entity_list[i].enemy)
			{
				return true;
			}
		}
	}
	return false;
}

Bool playerEnemyContact()
{
	int i;
	for (i = 1; i < entityManager.max_entities; i++)
	{
		if (entity_manager_collide(entityManager.entity_list[0], entityManager.entity_list[i]))
		{
			if (entityManager.entity_list[i].enemy)
			{
				return true;
			}
		}
	}
	return false;
}

Bool playerCollectContact()
{
	int i;
	for (i = 1; i < entityManager.max_entities; i++)
	{
		if (entity_manager_collide(entityManager.entity_list[0], entityManager.entity_list[i]))
		{
			if (entityManager.entity_list[i].collect)
			{
				return true;
			}
		}
	}
	return false;
}

Bool playerEnteranceContact()
{
	int i;
	for (i = 1; i < entityManager.max_entities; i++)
	{
		if (entity_manager_collide(entityManager.entity_list[0], entityManager.entity_list[i]))
		{
			if (entityManager.entity_list[i].enter)
			{
				return true;
			}
		}
	}
	return false;
}

void playerWarp()
{
	Entity a;
	Entity b;
	/*
	for (int i = 0; i < entityManager.max_entities; i++)
	{
		if (entityManager.entity_list[i].warp)
		{
			a = entityManager.entity_list[i];
			b = entityManager.entity_list[i + 1];
		}
	}*/

	if (entityManager.entity_list[3].warp && entityManager.entity_list[4].warp)
	{
		a = entityManager.entity_list[3];
		b = entityManager.entity_list[4];
	}
	
	if (entity_manager_collide(entityManager.entity_list[0], a))
	{
		entityManager.entity_list[0].position.x = b.position.x + 53;
		entityManager.entity_list[0].position.y = b.position.y;
	}
	if (entity_manager_collide(entityManager.entity_list[0], b))
	{
		entityManager.entity_list[0].position.x = a.position.x - 53;
		entityManager.entity_list[0].position.y = a.position.y;
	}
}
 
void entity_manager_update_entities()
{
	int i;
	if (entityManager.entity_list == NULL)
	{
		slog("entity does not exist");
		return;
	}
	for (i = 0; i < entityManager.max_entities; i++)
	{
		if (entityManager.entity_list[i]._inuse == 0)continue;
		entity_update(&entityManager.entity_list[i]);
		
	}
}

void entity_manager_draw_entities()
{
	int i;
	if (entityManager.entity_list == NULL)
	{
		slog("entity does not exist");
		return;
	}
	for (i = 0; i < entityManager.max_entities; i++)
	{
		if (entityManager.entity_list[i]._inuse == 0)continue;
		entity_draw(&entityManager.entity_list[i]);

	}
}


Entity *entity_new()
{
	int i;
	if (entityManager.entity_list == NULL)
	{
		slog("Entity system does not exists");
		return NULL;
	}

	for (i = 0; i < entityManager.max_entities; i++)
	{
		if (entityManager.entity_list[i]._inuse) continue; //it's already in use, so move on

		memset(&entityManager.entity_list[i], 0, sizeof(Entity));

		entityManager.entity_list[i]._inuse = 1;
		return &entityManager.entity_list[i];
	}
	slog("No entities available");
	return NULL;

}

void entityGravity(Entity *self)
{
	self->velocity.y += .5;

	if (entityGrounded())
	{
		if (self->velocity.y > 0)
		{
			self->velocity.y = 0;
		}
	}

}


void entity_free(Entity *ent)
{
	if (!ent)
	{
		slog("Cannot free a non-entity");
		return;
	}
	gf2d_sprite_free(ent->sprite);
	ent->sprite = 0;
	ent->_inuse = 0;
}


void entity_draw(Entity *ent)
{
	Vector2D drawPosition, offset;
	if (!ent)
	{
		slog("Cannot draw a NULL entity");
		return;
	}
	if (ent->draw) ent->draw(ent);
	else
	{
		if (ent->sprite == NULL)
		{
			return;
		}
		offset = cameraOffset();
		drawPosition = vector2d(ent->position.x + offset.x, ent->position.y + offset.y);
		gf2d_sprite_draw(
			ent->sprite,
			ent->position,
			NULL,   //Vector2D * scale,
			NULL,   //Vector2D * scaleCenter,
			&ent->rotation,   //Vector3D * rotation,
			&ent->flip,   //Vector2D * flip,
			NULL,   //Vector4D * colorShift,
			NULL);  //Uint32 frame);

	}
}