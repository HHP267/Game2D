#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <SDL.h>
#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

typedef struct Entity_s
{
	Bool _inuse;
	Vector2D position;
	Vector2D flip;
	Vector2D facing;
	Vector2D velocity;
	Vector2D acceleration;
	Vector3D rotation; //(x, y) - rotation center; z - degress of rotation
	Sprite *sprite;
	int w;
	int h;
	float frame;
	float frameRate;
	int frameCount;
	int worldState;
	void *data;
	void (*update)(struct Entity_s *self);
	void (*think)(struct Entity_s *self);
	void (*draw)(struct Entity_s *self);
	void (*free)(struct Entity_s *self);

	Bool enemy;
	Bool player;
	Bool platform;
	Bool collect;
	Bool enter;
	Bool destruct;
	Bool animate;
	Bool warp;
	Bool key;
}Entity;

/**
*	@brief initialize internal entity management system
*	@param max_entities how many concurrent entities to support
*/
void entity_manager_init(Uint32 max_entities);

/**
*	@brief frees the manager
*/
void entity_manager_free();

/**
* @brief calls update function on all entities
*/
void entity_manager_update_entities();


/*
*@brief checks if any entities in the entity manager collide
*/
Bool entity_manager_collide(Entity a, Entity b);

/*
*@brief returns a specific entity in the entity manager
*/
Entity entity_manager_return_entity(int i);

/*
* @brief calls draw function on all entities
*/
void entity_manager_draw_entities();

/**
*	@brief allocate an entity, intialize it, and set a pointer to it
*	@return NULL on error, or a pointer to an initialized property
*/
Entity *entity_new();

/*
*@brief applys a gravity vector to the entity
*/
void entityGravity(Entity *ent);

/*
*@brief checks if player touching the floor
*@return TRUE if touching floor, FALSE if not
*/
Bool entityGrounded();

/*
*@brief checks if enemy touching the floor
*@return TRUE if touching floor, FALSE if not
*/
Bool enemyGrounded();

/*
*@brief checks if something is on the same vertical as another entity
*@return TRUE if underneath, FALSE if not
*/
Bool playerUnderneath();

/*
*@brief checks if something is on the same horizontal another entity
*@return TRUE if on same horizontal, FALSE if not
*/
Bool playerHorizontal();

/*
*@brief checks if player touching the floor
*@return TRUE if touching enemy, FALSE if not
*/
Bool playerEnemyContact();

/*
*@brief checks if player touching the floor
*@return TRUE if touching collect, FALSE if not
*/
Bool playerCollectContact();

/*
*@brief checks if player touching the floor
*@return TRUE if touching enterance, FALSE if not
*/
Bool playerEnteranceContact();

/*
*@brief checks if player touching a destructable
*@param the destructable that is being contacted
*@return TRUE if touching a destructable, FALSE if not
*/
Bool playerDestructableContact(Entity *d);

/*
*@brief checks if destructable touching another destructable
*@param the destructable that is being contacted
*@return TRUE if touching a destructable, FALSE if not
*/
Bool destructablesContact(Entity *d);

/*
*@brief warps the player to the opposite warp point
*/
void playerWarp();

/*
*@brief removes the collectable item
*/
void collectRemove();

/**
*	@brief frees the entity
*	@param ent the entity to free
*/
void entity_free(Entity *ent);

/**
*	@brief draws the entity
*	@param ent the entity to draw
*/
void entity_draw(Entity *ent);

/**
*	@brief animates the entity
*	@param ent the entity to animate
*/
void entity_drawAnimate(Entity *ent);





#endif