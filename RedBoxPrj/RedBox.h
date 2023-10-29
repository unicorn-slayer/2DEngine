#pragma once
#include <SDL.h>
#include <vector>
#include "Rectangle.h"
#include "Level.h"

class Graphics;

class RedBox
{
public:
	RedBox();
	RedBox(Graphics& graphics, float x, float y);
	void handleEvent(SDL_Event& e);
	void update(const int& elapsedTime, const std::vector<Tile>& collisionTiles);
	void draw(Graphics& graphics, Rectangle& camera);
	void setCamera(Rectangle& camera);

private:
	float _x;
	float _y;
	float _dx;
	float _dy;
	float _speed;
	int _width;
	int _height;
	bool _jumping;
	bool _grounded;
	int _frameCounter;

	SDL_Rect _destRect;
	SDL_Texture* _redBox;

	Rectangle _rect;
};
