#ifndef __FONT_H__
#define __FONT_H__

#include "gfc_text.h"
#include "gfc_color.h"
#include "gfc_vector.h"
#include <SDL_ttf.h>
typedef struct
{
	TTF_Font *font;
	TextLine *file;
	int ptsize;
	Uint32 _refcount;
}Font;

/*
*@brief initialize font resources
*@param fonts to support
*/
void fontInit(Uint32 maxFonts);

Font *getByFilePoint(const char *filename, int ptsize);

/*
*@brief load the fonts
*@param the file with the fonts, the size of the fonts
*/
Font *fontLoad(const char *filename, int ptsize);

/*
*@brief frees the font
*@param fonts to support
*/
void fontFree(Font *font);

/*
*@brief puts the font onscreen
*@param fonts to support, text of the font, color of the font
*/
void fontRender(Font *font, char *text, Vector2D position, Color color);


#endif