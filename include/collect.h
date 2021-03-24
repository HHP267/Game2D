#ifndef __COLLECT_H__
#define __COLLECT_H__

#include "simple_logger.h"
#include <SDL.h>
#include "transition.h"
#include "entity.h"

/*
*@brief creates an collectable
*/
Entity *collectCreate(Vector2D position);

/*
*@brief updates the collectable/score
*/
void collectUpdate(Entity *self);

/*
*@brief returns the score of the player
*/
int scoreReturn();

#endif