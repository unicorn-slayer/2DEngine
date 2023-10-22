#include "RedBox.h"
#include "Graphics.h"
#include "Level.h"
#include "Globals.h"


RedBox::RedBox() : _x(0), _y(0), _dx(0), _dy(0), _speed(0.5), _width(0), _height(0), _jumping(false), _grounded(false), _frameCounter(0)
{
	_destRect = {};
	_rect = {};
	_redBox = NULL;
}

RedBox::RedBox(Graphics& graphics, float x, float y) : _x(x), _y(y), _dx(0), _dy(0), _speed(0.5), _width(32), _height(32), _jumping(false), _grounded(false), _frameCounter(0)
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
		case SDLK_LEFT: _dx -= _speed; break;
		case SDLK_RIGHT: _dx += _speed; break;
		case SDLK_SPACE:
			if (_grounded)
			{
				_jumping = true;
				_dy = -1.1f;
				_grounded = false;
			}
			break;

		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT: _dx += _speed; break;
		case SDLK_RIGHT: _dx -= _speed; break;
		case SDLK_SPACE:
			if (_jumping)
			{
     			_jumping = false;
				_dy = 0;
			}
			break;
		}
	}
}

void RedBox::update(const int& elapsedTime, const std::vector<Tile>& collisionTiles)
{
	// move x
	float startingX = _x;
	_x += (_dx * elapsedTime);
	_rect._x += (_dx * elapsedTime);

	if (_jumping) {
		if (_dy < 0)
		{
			_dy += 0.002f * elapsedTime;
		}
		else
		{
			_jumping = false;
		}
	}
	else {
		// apply gravity in y
		if (_dy < globals::g_gravityCap)
		{
			_dy += 0.002f * elapsedTime;
		}
	}

	// move y
	float startingY = _y;
	_y += (_dy * elapsedTime);
	_rect._y += (_dy * elapsedTime);

	std::vector<Tile> collisionTilesActive;

	// handle collisions
	for (const Tile& tile : collisionTiles)
	{
		if (_rect.checkCollision(tile))
		{
			collisionTilesActive.push_back(tile);
		}
	}

	if (!collisionTilesActive.empty())
	{
		for (Tile& tile : collisionTilesActive)
		{
			sides::Side side = _rect.getCollisionSide(tile.boundingBox);

			switch (side)
			{
			case sides::TOP:
				_dy = 0;
				_y = startingY;
				_rect._y = startingY;
				break;

			case sides::BOTTOM:
				_y = startingY;
				_rect._y = startingY;
				_dy = 0;
				_grounded = true;
				_frameCounter = 0;
				break;

			case sides::LEFT:
				//_x = startingX + 1;
				//_rect._x = startingX + 1;
				_x = tile.boundingBox._x + tile.boundingBox._width;
				_rect._x = tile.boundingBox._x + tile.boundingBox._width;
				break;

			case sides::RIGHT:
				//_x = startingX - 1;
				//_rect._x = startingX - 1;
				_x = tile.boundingBox._x - _rect._width;
				_rect._x = tile.boundingBox._x - _rect._width;
				break;
			}
		}
	}
	else
	{
		_frameCounter++;
	}

	// hack to make sure physiscs works.
	// When the player runs off the edge of a platform, he must lost ability to jump. Ideally, we'd just check to see if there
	// are any collisions with the bottom side of the player. If there are none, then _grounded becomes false. This is tricky
	// to implement. I first did a coarse method, where I just check if there are any collisions with the player. If there are no
	// collisions, i.e when playeer walks off platform, then _grounded should be false. However, for a reason that is something to do 
	// with timing, (maybe elapsed time is very small in an instance so change in y is too small to trigger a detection) sometimes no detections
	// are made, even when there should be. In this instance _grounded will still becomes false and the jump won't execute. The trick below
	// just essentially makes sure the player is most definitely not in contact with the ground, i.e. there has to be 10 confirmations
	// that the player is not in contact with ground before _grounded is false.
	// Even if this method is used, should still find a more accurate way of ensuring _grounded = false only when there are no BOTTOM
	// side collisions. At the moment this will happen when there are simply no collisions. Might still be ok.
	if (_frameCounter > 10)
	{
		_grounded = false;
	}

	// hanlde screen out of bounds in x
	if (_x < 0 || _x >(globals::g_screenWidth - globals::g_tileWidth))
	{
		_x = startingX;
		_rect._x = startingX;
	}

	// handle screen out of bounds in y
	if (_y < 0 || _y > (globals::g_screenWidth - globals::g_tileWidth))
	{
		_y = startingY;
		_rect._y = startingY;
		_dy = 0;
	}

	_destRect = { (int)_x, (int)_y, _width, _height };
}

void RedBox::draw(Graphics& graphics)
{
	graphics.blitSurface(_redBox, NULL, &_destRect);
}
