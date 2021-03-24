#ifndef __ENEMY_DROP_H__
#define __ENEMY_DROP_H__

#include "entity.h"

Entity *dropperSpawn(Vector2D position);

void dropperUpdate(Entity *self, Entity a);


#endif