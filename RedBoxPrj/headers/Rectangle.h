#pragma once
#include "SDL.h"
#include <vector>
#include "Globals.h"

struct Tile;

class Rectangle
{
public:

	Rectangle();
	Rectangle(float x, float y, int width, int height);

	bool checkCollision(const Rectangle& rect);
	sides::Side getCollisionSide(const Rectangle& other);

	float _x;
	float _y;
	int _width;
	int _height;
};