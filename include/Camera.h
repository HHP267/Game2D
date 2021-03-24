#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gfc_types.h"
#include "gfc_vector.h"

void cameraSetDimensions(Vector2D size);
void cameraSetPosition(Vector2D position);

void cameraMove(Vector2D move);

Bool cameraPoint(Vector2D point);
Bool cameraRect(SDL_Rect rect);

Vector2D cameraPosition();


Vector2D cameraOffset();



#endif