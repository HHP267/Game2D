#ifndef __DESTRUCTABLE_H__
#define __DESTRUCTABLE_H__

#include "simple_logger.h"
#include <SDL.h>
#include "entity.h"


/*
*@brief creates an rock
*/
Entity *rockCreate(Vector2D position);

/*
*@brief update for the rock
*/
void rockUpdate(Entity *self);

/*
*@brief creates the "key"
*/
Entity *plateCreate(Vector2D position);

/*
*@brief creates the "lock"
*/
Entity *lockCreate(Vector2D position);

/*
*@brief checks for collision between the plate and the destructable
*@param 2 entities
*@return true if the collision happens between the plate and a destructable. False otherwise
*/
Bool plateUnlock(Entity *a, Entity *b);




#endif