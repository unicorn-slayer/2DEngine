#include "RedBox.h"
#include "Graphics.h"
#include "Level.h"
#include "Globals.h"

RedBox::RedBox(Graphics& graphics, float x, float y) : _x(x), _y(y), _accelX(0), _accelY(0), _accelerationMagX(0.004f), _accelerationMagY(0.002f), _width(32), _height(32), _jumping(false), _grounded(false), _frameCounter(0),
_currentAnimation(""), _frameIndex(0), _timeUpdate(50.0f), _timeElapsed(0), _rightHeld(false), _leftHeld(false)
{
	_redBox = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage("red_icon.png"));
	_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage("goomba.png"));
	_destRect = {};
	_rect = { _x, _y, _width, _height};
	this->setupAnimation();
	this->playAnimation("goombaStop");

	playerVelocityX = 0;
	playerVelocityY = 0;

}

void RedBox::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT: 
			_accelX = -_accelerationMagX;
			_leftHeld = true;
			break;
		case SDLK_RIGHT:
			_accelX = _accelerationMagX;
			_rightHeld = true;
			break;
		case SDLK_SPACE:
			if (_grounded)
			{
				_jumping = true;
				playerVelocityY = -1.1f;
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
		case SDLK_LEFT:
			_accelX = _accelerationMagX;
			_leftHeld = false;
			break;
		case SDLK_RIGHT:
			_accelX = -_accelerationMagX;
			_rightHeld = false;
			break;
		case SDLK_SPACE:
			if (_jumping)
			{
     			_jumping = false;
				if (playerVelocityY < 0)
				{
					playerVelocityY = 0;
				}

			}
			break;
		}
	}

	if (_leftHeld || _rightHeld)
	{
		this->playAnimation("goombaWalk");
	}
	else
	{
		this->playAnimation("goombaStop");
	}

	if (!_grounded)
	{
		this->playAnimation("goombaStop");
	}
}

void RedBox::update(const float& elapsedTime, const std::vector<Tile>& collisionTiles)
{
	// move x
	float startingX = _x;
	float maxSpeed = 0.8f;


	// Accelerate to the right

	if (_leftHeld || _rightHeld)
	{
		playerVelocityX += _accelX * elapsedTime;

		// Limit the player's velocity to the maximum speed
		if (playerVelocityX > maxSpeed) {
			playerVelocityX = maxSpeed;
		}
		else if (playerVelocityX < -maxSpeed) {
			playerVelocityX = -maxSpeed;
		}
	}
	else
	{
		if (playerVelocityX > 0)
		{
			playerVelocityX += _accelX * elapsedTime;

			if (playerVelocityX < 0)
			{
				playerVelocityX = 0;
			}
		}
		if (playerVelocityX < 0)
		{
			playerVelocityX += _accelX * elapsedTime;

			if (playerVelocityX > 0)
			{
				playerVelocityX = 0;
			}
		}
	}

	// Update player position
	_x += playerVelocityX * elapsedTime;
	_rect._x += playerVelocityX * elapsedTime;


	if (playerVelocityY < globals::g_gravityCap)
	{
		playerVelocityY += _accelerationMagY * elapsedTime;

	}

	// move y
	float startingY = _y;
	_y += (playerVelocityY * elapsedTime);
	_rect._y += (playerVelocityY * elapsedTime);

	std::vector<Tile> collisionTilesActive;

	// handle collisions
	for (const Tile& tile : collisionTiles)
	{
		if (_rect.checkCollision(tile.boundingBox))
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
				playerVelocityY = 0;
				_y = startingY;
				_rect._y = startingY;
				break;

			case sides::BOTTOM:
				_y = startingY;
				_rect._y = startingY;
				playerVelocityY = 0;
				_grounded = true;
				_frameCounter = 0;
				break;

			case sides::LEFT:
				//_x = startingX + 1;
				//_rect._x = startingX + 1;
				_x = (tile.boundingBox._x + tile.boundingBox._width)+2;
				_rect._x = (tile.boundingBox._x + tile.boundingBox._width)+2;
				playerVelocityX = 0;
				//_dx = 0;
				break;

			case sides::RIGHT:
				//_x = startingX - 1;
				//_rect._x = startingX - 1;
				_x = (tile.boundingBox._x - _rect._width)-2;
				_rect._x = (tile.boundingBox._x - _rect._width)-2;
				playerVelocityX = 0;
				//_dx = 0;
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
	if (_x < 0 || _x >(globals::g_mapWidth - globals::g_tileWidth))
	{
		_x = startingX;
		_rect._x = startingX;
	}

	// handle screen out of bounds in y
	if (_y < 0 || _y > (globals::g_mapWidth - globals::g_tileWidth))
	{
		_y = startingY;
		_rect._y = startingY;
		playerVelocityY = 0;
	}

	_destRect = { (int)_x, (int)_y, _width, _height };

	this->animationUpdate((float)elapsedTime);
}

void RedBox::draw(Graphics& graphics, Rectangle& camera)
{
	//when drawing, need to offset the redBox by the camera distance
	_destRect.x = _destRect.x - (int)camera._x;
	_destRect.y = _destRect.y - (int)camera._y;

	//graphics.blitSurface(_redBox, NULL, &_destRect);

	this->animateDraw(graphics, (int)_destRect.x, (int)_destRect.y);
}

void RedBox::setCamera(Rectangle& camera)
{
	//Center the camera over the dot, 700 is screen width
	camera._x = (_rect._x + (_width / 2)) - (globals::g_screenWidth/2);
	camera._y = (_rect._y + (_height / 2)) - (globals::g_screenHeight/2);

	//Keep the camera in bounds
	if (camera._x < 0)
	{
		camera._x = 0;
	}
	if (camera._y < 0)
	{
		camera._y = 0;
	}

	//screen width needs to be changed to level width below
	if (camera._x > globals::g_mapWidth - camera._width)
	{
		camera._x = float(globals::g_mapWidth - camera._width);
	}
	if (camera._y > globals::g_mapHeight - camera._height)
	{
		camera._y = float(globals::g_mapHeight - camera._height);
	}
}

void RedBox::addAnimation(int frames, int x, int y, std::string name, int width, int height)
{
	std::vector<SDL_Rect> rectangles;
	for (int i = 0; i < frames; i++) {
		SDL_Rect newRect = { i * width, y, width, height };
		rectangles.push_back(newRect);
	}
	_animations.insert(std::pair<std::string, std::vector<SDL_Rect> >(name, rectangles));
}

void RedBox::playAnimation(std::string animation)
{
	if (_currentAnimation != animation) {
		_currentAnimation = animation;
		_frameIndex = 0;
	}
}

void RedBox::animationUpdate(float elapsedTime)
{
	_timeElapsed += elapsedTime;
	if (this->_timeElapsed > this->_timeUpdate) {
		this->_timeElapsed -= this->_timeUpdate;
		if (this->_frameIndex < this->_animations[this->_currentAnimation].size() - 1) {
			this->_frameIndex++;
		}
		else {
			this->_frameIndex = 0;
		}
	}
}

void RedBox::animateDraw(Graphics& graphics, int x, int y)
{
	SDL_Rect destinationRectangle;
	destinationRectangle.x = x;
	destinationRectangle.y = y;
	destinationRectangle.w = _rect._width;
	destinationRectangle.h = _rect._height;

	SDL_Rect sourceRect = _animations[this->_currentAnimation][this->_frameIndex];
	graphics.blitSurface(_spriteSheet, &sourceRect, &destinationRectangle);
}

void RedBox::setupAnimation()
{
	this->addAnimation(2, 0, 0, "goombaWalk", 16, 16);
	this->addAnimation(1, 0, 0, "goombaStop", 16, 16);
}
