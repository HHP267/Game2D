#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gfc_text.h"
#include "gfc_types.h"
#include "entity.h"
#include "gf2d_sprite.h"

typedef enum
{
	tileEmpty,
	tileBrick,
	tile
}TileTypes;

typedef struct
{
	Sprite *bgimage; /**<the background image for the level*/
	Sprite *tilesetA; /**<the tile image for the level*/
	TileTypes *tileMap; /**<how many tiles for the level*/
	int levelWidth; /**<tiles per row for the level*/
	int levelHeight; /**<tiles per column for the level*/
	int tileWidth; /**<how wide each tile is*/
	int tileHeight; /**<how tall each tile is*/
	int tileFramesPerLine; /**<how many frames per line for each tile*/
}Level;

/**
*@brief create the memory for a level
*/
Level *levelNew();

/**
*@brief load the level from JSON file
*@param filename of file to load
*@returns NULL on error, a loaded level otherwise
*/
Level *levelLoad();


/**
* @brief free up a previously loaded level
*/
void levelFree();


/**
* @brief draw a level
*/
void levelDraw();





#endif