#ifndef __TRANSITION_H_
#define __TRANSITION_H_

#include "entity.h"

/*
* @brief spawn in transition entity
* @param position the screen position to spawn the player
* @return NULL on error, or a pointer to the new player entity
*/
Entity *transitionLocationSpawn(Vector2D position);


void transitionUpdate(Entity *self);

/*
*@brief returns the world state integer value
*@return returns an integer, 0 (for overworld) or 1 (for levels)
*/
int getWorldState();

/*
*@brief changes the world state integer value
*/
void changeWorldState(int levelType);


#endif