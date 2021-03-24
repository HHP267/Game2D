#ifndef __WARP_H_
#define __WARP_H_

#include "entity.h"

/*
* @brief spawn in transition entity
* @param position the screen position to spawn the player
* @return NULL on error, or a pointer to the new player entity
*/
Entity *warpPointSpawn(Vector2D position);



#endif