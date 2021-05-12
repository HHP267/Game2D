#include "shape.h"
#include "gfc_types.h"


ShapeRect rectFromSDL(SDL_Rect r)
{
	ShapeRect sr;
	
	gfc_rect_set(sr, r.x, r.y, r.w, r.h);
	
	return sr;
}

ShapeRect rectFromVector(Vector4D v)
{
	ShapeRect sr;

	gfc_rect_set(sr, v.x, v.y, v.z, v.w);

	return sr;
}

ShapeRect createRect(float x, float y, float w, float h)
{
	ShapeRect sr;

	sr.x = x;
	sr.y = y;
	sr.w = w;
	sr.h = h;

	return sr;
}

/*
SDL_Rect rectTOSDL(ShapeRect r)
{
	return gfc_sdl_rect(r.x, r.y, r.w, r.h);
} */

Bool shapePointRect(ShapeEdge point, ShapeRect r)
{
	if ((point.x1 < r.x) || (point.x1 > r.x + r.w) || (point.y1 < r.y) || (point.y1 > r.y + r.h))
	{
		return false;
	}
	return true;
}

Bool shapeRectCollide(ShapeRect a, ShapeRect b)
{
	if (((a.x + a.w) < b.x) || (a.x > (b.x + b.w)) || ((a.y + a.h) < b.y) || (a.y > (b.y + b.h)))
	{
		return false;
	}
	return true;
}

ShapeCircle circleBuild(float x, float y, float r)
{
	ShapeCircle c;
	c.x = x;
	c.y = y;
	c.r = r;

	return c;
}


Bool shapeCircleCollide(ShapeCircle a, ShapeCircle b)
{
	float radiusSqr = (a.r * a.r) + (b.r * b.r);
	float cSqr = (((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)));

	if (radiusSqr <= cSqr)
	{
		return false;
	}
	return true;
}

Bool shapePointCircle(ShapeEdge point, ShapeCircle c)
{
	return shapeCircleCollide(circleBuild(point.x1, point.y1, 0), c);
}