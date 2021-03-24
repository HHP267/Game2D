#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "gfc_types.h"
#include "gfc_vector.h"

typedef struct
{
	float x, y, w, h;
}ShapeRect;

typedef struct
{
	float x, y, r;
}ShapeCircle;

typedef Vector2D ShapePoint;


ShapeRect rectFromSDL(SDL_Rect r);
ShapeRect rectFromVector(Vector4D v);

SDL_Rect rectTOSDL(ShapeRect r);


/*
* @brief check if a point is inside a rectangle
* @param point in question
* @param rectangle in question
* @return true if inside, false if not
*/
Bool shapePointRect(ShapePoint point, ShapeRect r);

/*
* @brief check if rectangles overlap
* @param rectangle A in question
* @param rectangle B in question
* @return true if collide, false if no
*/
Bool shapeRectCollide(ShapeRect a, ShapeRect b);

/*
* @brief check if circles overlap
* @param circle A in question
* @param circle B in question
* @return true if collide, false if no
*/
Bool shapeCircleCollide(ShapeCircle a, ShapeCircle b);

/*
* @brief check if a point is inside a circle
* @param point in question
* @param circle in question
* @return true if inside, false if not
*/
Bool shapePointCircle(ShapePoint point, ShapeCircle c);






#endif