#include "Camera.h"

static SDL_Rect camera = { 0 };


void cameraSetDimensions(Vector2D size)
{
	camera.w = size.x;
	camera.h = size.y;
}

void cameraSetPosition(Vector2D position)
{
	camera.x = position.x;
	camera.y = position.y;
}

void cameraMove(Vector2D move);

Vector2D cameraPosition()
{
	Vector2D position;

	position.x = camera.x;
	position.y = camera.y;

	return position;
}

Vector2D cameraOffset()
{
	Vector2D position;

	position.x = -camera.x;
	position.y = -camera.y;

	return position;
}

Bool cameraPoint(Vector2D point)
{
	if (point.x < camera.x || point.x > (camera.x + camera.w) || point.y < camera.y || point.y > (camera.y + camera.h))
	{
		return false;
	}
	return true;
}

Bool cameraRect(SDL_Rect rect)
{
	if ((rect.x + rect.w) < camera.x || rect.x > (camera.x + camera.w) || (rect.y + rect.h) < camera.y || rect.y > (camera.y + camera.h))
	{
		return false;
	}
	return true;
}

