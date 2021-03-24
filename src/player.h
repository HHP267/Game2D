#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

/*
* @briefs spawn in player entity
* @param position the screen position to spawn the player
* @return NULL on error, or a pointer to the new player entity
*/
Entity *playerSpawn(Vector2D position);

/*
* @briefs holds the player inside the screen boundaries
* @param a pointer to the player entity
*/
void playerBoundaries(Entity *self);

/*
* @briefs locates the player's position and places that into a Vector2D value
* @param a pointer to the player entity
*/
void playerFindPosition(Entity *self);

/*
* @briefs checks where the player entity is an returns that location
* @return a Vector2D (x, y) of where the player is onscreen
*/
Vector2D returnPlayerPosition();

/*
* @briefs returns the total score of the player
* @return a score integer
*/
float returnScore();

/*
* @briefs updates the total score of the player
*/
void updateScore();

/*
* @briefs updates the player entity via control and contact
* @param a pointer to the player entity
*/
void playerUpdate(Entity *self);

/*
* @briefs kills the player entity
*/
void playerDie();


#endif
