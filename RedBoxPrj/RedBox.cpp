#include "RedBox.h"
#include "Graphics.h"
#include "Level.h"
#include "Globals.h"


RedBox::RedBox() : _x(0), _y(0), _dx(0), _dy(0), _speed(0.5), _width(0), _height(0), _jumping(false)
{
	_destRect = {};
	_rect = {};
	_redBox = NULL;
}

RedBox::RedBox(Graphics& graphics, float x, float y) : _x(x), _y(y), _dx(0), _dy(0), _speed(0.5), _width(32), _height(32), _jumping(false)
{
	_redBox = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage("red_icon.png"));
	_destRect = {};
	_rect = { _x, _y, _width, _height};
}

void RedBox::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		//case SDLK_UP: _dy -= _speed; break;
		//case SDLK_DOWN: _dy += _speed; break;
		case SDLK_LEFT: _dx -= _speed; break;
		case SDLK_RIGHT: _dx += _speed; break;
		case SDLK_SPACE: _jumping = true;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		//case SDLK_UP: _dy += _speed; break;
		//case SDLK_DOWN: _dy -= _speed; break;
		case SDLK_LEFT: _dx += _speed; break;
		case SDLK_RIGHT: _dx -= _speed; break;
		case SDLK_SPACE: _jumping = false;
		}
	}
}

void RedBox::update(const int& elapsedTime, const std::vector<Tile>& collisionTiles)
{
	// move x
	float startingX = _x;
	_x += (_dx * elapsedTime);
	_rect._x += (_dx * elapsedTime);

	// handle collisions in x
	for (const Tile& tile : collisionTiles)
	{
		if (_rect.checkCollision(tile))
		{
			_x = startingX;
			_rect._x = startingX;
		}
	}

	// hanlde screen out of bounds in x
	if (_x < 0 || _x >(globals::g_screenWidth - globals::g_tileWidth))
	{
		_x = startingX;
		_rect._x = startingX;
	}

	// apply gravity in y
	if (_dy < globals::g_gravityCap)
	{
		_dy += 0.002f * elapsedTime;
	}

	// move y
	float startingY = _y;
	_y += (_dy * elapsedTime);
	_rect._y += (_dy * elapsedTime);

	if (_jumping)
	{
		_dy = 0;
		_dy -= 0.8f;
	}

	// handle collisions in y
	for (const Tile& tile : collisionTiles)
	{
		if (_rect.checkCollision(tile))
		{
			_y = startingY;
			_rect._y = startingY;
		}
	}

	// handle screen out of bounds in y
	if (_y < 0 || _y > (globals::g_screenWidth - globals::g_tileWidth))
	{
		_y = startingY;
		_rect._y = startingY;
	}

	_destRect = { (int)_x, (int)_y, _width, _height };
}

void RedBox::draw(Graphics& graphics)
{
	graphics.blitSurface(_redBox, NULL, &_destRect);
}
