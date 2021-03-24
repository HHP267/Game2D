#ifndef __ENEMY_CHASE_H__
#define __ENEMY_CHASE_H__

#include "entity.h"

Entity *chaserSpawn(Vector2D position);

void chaserUpdate(Entity *self);


#endif