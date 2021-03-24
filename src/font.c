#include "font.h"
#include "gfc_text.h"
#include "gfc_color.h"
#include "gf2d_graphics.h"
#include "simple_logger.h"

typedef struct
{
	Font *fontList;
	Uint32 maxFonts;
}FontManager;


static FontManager fontManager = {0};


void fontDelete(Font *font)
{
	if (!font)
	{
		return;
	}
	if (font->font != NULL)TTF_CloseFont(font->font);
	memset(font, 0, sizeof(Font));
}

void fontManagerClose()
{
	int i;
	if (fontManager.fontList != NULL)
	{
		for (i = 0; i < fontManager.maxFonts; i++)
		{
			fontDelete(&fontManager.fontList[i]);
		}
	}
	free(fontManager.fontList);
	memset(&fontManager.fontList, 0, sizeof(FontManager));
}

void fontInit(Uint32 maxFonts)
{
	if (!maxFonts)
	{
		slog("cannot allocate for 0 fonts");
		return;
	}
	if ((!TTF_WasInit()) && (TTF_Init() == -1))
	{
		slog("TTF_Init: %s\n", TTF_GetError());
		return;
	}

	fontManagerClose();
	fontManager.maxFonts = maxFonts;
	fontManager.fontList = (Font *)gfc_allocate_array(sizeof(Font), maxFonts);


	atexit(fontManagerClose);
}

Font *fontNew()
{
	int i;
	for (i = 0; i < fontManager.maxFonts; i++)
	{
		if (fontManager.fontList[i]._refcount == 0)
		{
			fontManager.fontList[i]._refcount = 1;
			return &fontManager.fontList[i];
		}
	}
	return NULL;
}

Font *getByFilePoint(const char *filename, int ptsize){
	int i;
	//slog("starting getByFilePoint loop");
	for (i = 0; i < fontManager.maxFonts; i++)
	{
		if (fontManager.fontList[i]._refcount == 0)continue;
		slog("passed through first checks");
		if (gfc_line_cmp(fontManager.fontList[i].file, filename) != 0)continue;
		slog("passed through second checks");
		if (fontManager.fontList[i].ptsize != ptsize)continue;
		slog("passed through all checks");
		return &fontManager.fontList[i];
	}
	return NULL;
}

Font *fontLoad(const char *filename, int ptsize)
{
	Font *font;
	font = getByFilePoint(filename, ptsize);
	slog("getByFilePoint function passed");
	if (font != NULL)
	{
		font->_refcount++; //this font has been found already
		return font;
	}

	font = fontNew();
	slog("fontNew passed");
	if (font == NULL)
	{
		slog("failed to get a new font slot");
		return NULL;
	}

	font->ptsize = ptsize;

	//gfc_line_cpy(font->file, filename);
	//strncpy(font->file, filename, 128);
	font->file = filename;
	slog("font->file copied");

	font->font = TTF_OpenFont(filename, ptsize);
	if (font->font == NULL)
	{
		slog("failed to open font %s", filename);
		fontDelete(font);
		return NULL;
	}

	return font;
}

void fontFree(Font *font)
{
	if (!font)return;

	font->_refcount--;

	if (font->_refcount == 0)
	{
		fontDelete(font);
	}
}

void fontRender(Font *font, char *text, Vector2D position, Color color)
{
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Rect rect;
	if (!font || !font->font)
	{
		slog("no font provided to render");
		return;
	}
	if (!text)
	{
		slog("no text provided to render");
		return;
	}

	surface = TTF_RenderText_Blended(font->font, (const char *)text, gfc_color_to_sdl(color));

	if (!surface)
	{
		slog("failed to render font text");
		return;
	}

	surface = gf2d_graphics_screen_convert(&surface);
	if (!surface)
	{
		slog("failed to convert rendered text to graphics format %s", text);
		return;
	}

	texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surface);
	if (!texture)
	{
		slog("failed to convert surface text to texture %s", text);
		SDL_FreeSurface(surface);
		return;
	}
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	rect = gfc_sdl_rect((Sint32)position.x, (Sint32)position.y, surface->w, surface->h);

	SDL_UpdateTexture(texture,
		NULL,
		surface->pixels,
		surface->pitch);
	SDL_RenderCopyEx(gf2d_graphics_get_renderer(),
		texture,
		NULL,
		&rect,
		0,
		NULL,
		0);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);

}