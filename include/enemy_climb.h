#ifndef __ENEMY_JUMP_H__
#define __ENEMY_JUMP_H__

#include "entity.h"

Entity *climberSpawn(Vector2D position);

void climberUpdate(Entity *self);


#endif