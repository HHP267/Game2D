#ifndef __FLOOR_H__
#define __FLOOR_H__

#include "entity.h"

/*
* @briefs spawn in player entity
* @param position the screen position to spawn the player
* @return NULL on error, or a pointer to the new player entity
*/
Entity *floorCreate(Vector2D position);

void entityFloor(Entity character, Entity floor);

#endif