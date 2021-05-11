#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "gfc_types.h"
#include "gfc_vector.h"

typedef struct
{
	float x, y, w, h;
}Rect;

typedef struct
{
	float x, y, r;
}Circle;

typedef Vector2D ShapePoint;


Rect rectFromSDL(SDL_Rect r);
Rect rectFromVector(Vector4D v);

SDL_Rect rectTOSDL(Rect r);


/*
* @brief check if a point is inside a rectangle
* @param point in question
* @param rectangle in question
* @return true if inside, false if not
*/
Bool shapePointRect(ShapePoint point, Rect r);

/*
* @brief check if rectangles overlap
* @param rectangle A in question
* @param rectangle B in question
* @return true if collide, false if no
*/
Bool shapeRectCollide(Rect a, Rect b);

/*
* @brief check if circles overlap
* @param circle A in question
* @param circle B in question
* @return true if collide, false if no
*/
Bool shapeCircleCollide(Circle a, Circle b);

/*
* @brief check if a point is inside a circle
* @param point in question
* @param circle in question
* @return true if inside, false if not
*/
Bool shapePointCircle(ShapePoint point, Circle c);


#endif