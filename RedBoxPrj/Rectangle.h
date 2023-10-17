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

	// std::vector<Rectangle> checkCollision(const std::vector<Tile>& collisionRects);
	bool checkCollision(const Tile& tile);
	sides::Side getCollisionSide(Rectangle& other);

	float _x;
	float _y;
	int _width;
	int _height;
};