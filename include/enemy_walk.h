#ifndef __ENEMY_WALK_H__
#define __ENEMY_WALK_H__

#include "entity.h"
#include "player.h"


Entity *walkerSpawn(Vector2D position);

Vector2D walkerGetPosition(Entity *self);

void walkerUpdate(Entity *self);


#endif