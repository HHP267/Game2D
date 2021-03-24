#include "level.h"
#include "camera.h"
#include "simple_logger.h"
#include "simple_json.h"
#include "entity.h"
#include "floor.h"
#include "camera.h"
#include <stdlib.h>

Level *levelNew()
{
	Level *level;
	level = (Level *)malloc(sizeof(Level));

	if (!level)
	{
		slog("Failed to allocate memory for the game level");
		return NULL;
	}
	memset(level, 0, sizeof(Level));
	return level;
}


Level *levelLoad(const char *filename)
{
	const char *string;
	Level *level;
	SJson *json, *leveljs, *levelMap, *row;
	int rows, cols, i, j, count, tileIndex;
	entity_manager_init(100);

	if (!filename)
	{
		slog("filename is NULL, cannot load level");
		return NULL;
	}

	json = sj_load(filename);
	if (!json) return NULL;

	level = levelNew();
	if (!level)
	{
		return NULL;
	}

	leveljs = sj_object_get_value(json, "level");
	if (!leveljs)
	{
		slog("Missing Level JSON object");
		levelFree(level);
		sj_free(json);
		return NULL;
	}
	string = sj_get_string_value(sj_object_get_value(leveljs, "bgimage"));
	if (string)
	{
		level->bgimage = gf2d_sprite_load_image((char *)string);
	}

	string = sj_get_string_value(sj_object_get_value(leveljs, "tileset"));
	if (string)
	{
		sj_get_integer_value(sj_object_get_value(leveljs, "tileWidth"), &level->tileWidth);
		sj_get_integer_value(sj_object_get_value(leveljs, "tileHeight"), &level->tileHeight);
		sj_get_integer_value(sj_object_get_value(leveljs, "tileFramesPerLine"), &level->tileFramesPerLine);

		level->tilesetA = gf2d_sprite_load_all(
			(char *)string,
			level->tileWidth,
			level->tileHeight,
			level->tileFramesPerLine);
	}

	levelMap = sj_object_get_value(leveljs, "tileMap");
	if (!levelMap)
	{
		slog("missing tileMap data");
		levelFree(level);
		sj_free(json);
		return NULL;
	}
	rows = sj_array_get_count(levelMap);
	row = sj_array_get_nth(levelMap, 0);
	cols = sj_array_get_count(row);
	count = rows*cols;
	level->levelWidth = cols;
	level->levelHeight = rows;

	level->tileMap = (TileTypes *)gfc_allocate_array(sizeof(TileTypes), count);
	if (!level->tileMap)
	{
		levelFree(level);
		sj_free(json);
		return NULL;
	}

	tileIndex = 0;
	for (j = 0; j < rows; j++)
	{
		row = sj_array_get_nth(levelMap, j);
		if (!row)continue;

		if (cols != sj_array_get_count(row))
		{
			slog("row %i, cols count mismatch");
			continue;
		}
		for (i = 0; i < cols; i++)
		{
			sj_get_integer_value(sj_array_get_nth(row, i), &level->tileMap[tileIndex++]);
		}

	}

	sj_free(json);
	return level;
}


void levelFree(Level *level)
{
	if (!level)
	{
		return;
	}

	if (level->tileMap != NULL)
	{
		free(level->tileMap);
		level->tileMap = NULL;
	}
	gf2d_sprite_free(level->bgimage);
	gf2d_sprite_free(level->tilesetA);
	

	free(level);
}

void levelDraw(Level *level)
{
	int i;
	if (!level)
	{
		slog("Cannot Draw Level - Null pointer provided");
		return;
	}

	//draw BG
	gf2d_sprite_draw_image(level->bgimage, vector2d(0, 0));


	if (!level->tileMap)
	{
		slog("no tiles loaded for level");
		return;
	}

	//draw tiles
	for (i = 0; i < level->levelWidth * level->levelHeight; i++)
	{
		if (level->tileMap[i] == 0) continue;

		gf2d_sprite_draw(
			level->tilesetA,
			vector2d((i%level->levelWidth) * level->tilesetA->frame_w, (i/level->levelHeight) * level->tilesetA->frame_h),
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			level->tileMap[i] - 1);
	}
}






/*footer*/