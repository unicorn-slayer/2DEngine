#include <algorithm>
#include "Goomba.h"

Goomba::Goomba(Graphics& graphics, float x, float y)
	: AnimatedSprite(graphics, "goomba.png", 0, 0, 32, 32, x, y, 5)
	, m_accelX(0)
	, m_accelY(0)
	, m_accelerationMagX(0.8f) //0.008f
	, m_accelerationMagY(0.5f) //0.002f //0.5f
	, m_jumping(false)
	, m_grounded(false)
	, m_frameCounter(0)
	, m_rightHeld(false)
	, m_leftHeld(false)
	, m_playerVelocityX(0)
	, m_playerVelocityY(0)

{
	this->setupAnimation();
	AnimatedSprite::playAnimation("goombaStop");
}

void Goomba::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			m_accelX = -m_accelerationMagX;
			m_leftHeld = true;
			break;

		case SDLK_RIGHT:
			m_accelX = m_accelerationMagX;
			m_rightHeld = true;
			break;

		case SDLK_SPACE:
			if (m_grounded)
			{
				m_jumping = true;
 				m_playerVelocityY = -18.0f;
				m_grounded = false;
			}
			break;
		}
	}

	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			m_accelX = m_accelerationMagX;
			m_leftHeld = false;
			break;

		case SDLK_RIGHT:
			m_accelX = -m_accelerationMagX;
			m_rightHeld = false;
			break;

		case SDLK_SPACE:
			if (m_jumping)
			{
				m_jumping = false;
				if (m_playerVelocityY < 0)
				{
					m_playerVelocityY = 0;
				}
			}
			break;
		}
	}
}

void Goomba::update(const float& elapsedTime, const std::vector<Tile>& collisionTiles)
{
	this->move(elapsedTime);

	this->handleCollisions(collisionTiles);

	this->handleScreenBounds(elapsedTime);

	this->doAnimations();

	this->animationUpdate(elapsedTime);

	this->updateGoombaDataQueue();

}

void Goomba::move(const float& elapsedTime)
{
	// move x
	float maxSpeed = 15.0f; /////////////////////////////////////////////////////////////////////

	if (m_leftHeld || m_rightHeld)
	{
		m_playerVelocityX += m_accelX * elapsedTime;

		m_playerVelocityX = std::clamp(m_playerVelocityX, -maxSpeed, maxSpeed);
	}
	else
	{
		if (m_accelX <= 0)
		{
			m_playerVelocityX += m_accelX * elapsedTime;

			m_playerVelocityX = std::max(0.0f, m_playerVelocityX);
		}

		if (m_accelX >= 0)
		{
			m_playerVelocityX += m_accelX * elapsedTime;

			m_playerVelocityX = std::min(0.0f, m_playerVelocityX);
		}
	}

	// Update x position
	m_x += m_playerVelocityX * elapsedTime;
	m_boundingBox._x += m_playerVelocityX * elapsedTime;

	// apply gravity
	if (m_playerVelocityY < globals::g_gravityCap) ///////////////////////////////////////////////////////////
	{
		m_playerVelocityY += m_accelerationMagY * elapsedTime;
	}

	// Update y position
	m_y += (m_playerVelocityY * elapsedTime);
	m_boundingBox._y += (m_playerVelocityY * elapsedTime);


}

void Goomba::updateGoombaDataQueue()
{
	constexpr int MAX_ELEMENTS = 31;


	if (m_goombaDataQueue.size() < MAX_ELEMENTS)
	{
		goombaData goomba(m_x, m_y, m_grounded, m_playerVelocityX, m_rightHeld, m_leftHeld);

		m_goombaDataQueue.emplace(goomba);
	}
}

void Goomba::handleCollisions(const std::vector<Tile>& collisionTiles)
{
	std::vector<Tile> collisionTilesActive;

	// handle collisions
	for (const Tile& tile : collisionTiles)
	{
		if (Sprite::m_boundingBox.checkCollision(tile.boundingBox))
		{
			collisionTilesActive.push_back(tile);			
		}
	}

	if (!collisionTilesActive.empty())
	{
		for (Tile& tile : collisionTilesActive)
		{
  			sides::Side side = m_boundingBox.getCollisionSide(tile.boundingBox);

			switch (side)
			{
			case sides::TOP:
				m_playerVelocityY = 0;
				Sprite::m_y = tile.boundingBox._y + globals::g_tileHeight + 1;
				Sprite::m_boundingBox._y = tile.boundingBox._y + globals::g_tileHeight + 1;
				break;

			case sides::BOTTOM:
				Sprite::m_y = tile.boundingBox._y - globals::g_tileHeight;
				Sprite::m_boundingBox._y = tile.boundingBox._y - globals::g_tileHeight;
				m_playerVelocityY = 0;
				m_grounded = true;
				m_frameCounter = 0;
				break;

			case sides::LEFT:
				Sprite::m_x = (tile.boundingBox._x + tile.boundingBox._width) + 2;
				Sprite::m_boundingBox._x = (tile.boundingBox._x + tile.boundingBox._width) + 2;
				m_playerVelocityX = 0;
				break;

			case sides::RIGHT:
				Sprite::m_x = (tile.boundingBox._x - this->m_boundingBox._width) - 2;
				Sprite::m_boundingBox._x = (tile.boundingBox._x - this->m_boundingBox._width) - 2;
				m_playerVelocityX = 0;
				break;
			}
		}
	}
	else
	{
		m_frameCounter++;
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
	if (m_frameCounter > 100)
	{
		m_grounded = false;
	}
}

//handles screen going out of bounds
void Goomba::handleScreenBounds(const float& elapsedTime)
{
	float max_x = globals::g_mapWidth - globals::g_tileWidth;
	float max_y = globals::g_mapHeight - globals::g_tileHeight;

	// handle screen out of bounds in x
	if (Sprite::m_x < 0)
	{
		Sprite::m_x = 0;
		Sprite::m_boundingBox._x = 0;
		m_playerVelocityX = 0;
	}

	if (Sprite::m_x > max_x)
	{
		Sprite::m_x = max_x;
		Sprite::m_boundingBox._x = max_x;
		m_playerVelocityX = 0;
	}

	// handle screen out of bounds in y
	if (Sprite::m_y < 0)
	{
		Sprite::m_y = 0;
		Sprite::m_boundingBox._y = 0;
		m_playerVelocityY = 0;
	}

	if (Sprite::m_y > max_y)
	{
		Sprite::m_y = max_y;
		Sprite::m_boundingBox._y = max_y;
		m_playerVelocityY = 0;
	}
}

void Goomba::draw(Graphics& graphics, Rectangle& camera)
{
	SDL_Rect destRect = { (int)Sprite::m_x, (int)Sprite::m_y, globals::g_tileWidth, globals::g_tileHeight };

	//when drawing, need to offset the redBox by the camera distance
	destRect.x = destRect.x - (int)camera._x;
	destRect.y = destRect.y - (int)camera._y;

	AnimatedSprite::animateDraw(graphics, (int)destRect.x, (int)destRect.y);
}

void Goomba::setCamera(Rectangle& camera)
{
	//Center the camera over the dot, 700 is screen width
	camera._x = (Sprite::m_boundingBox._x + (globals::g_tileWidth / 2)) - (globals::g_screenWidth / 2);
	camera._y = (Sprite::m_boundingBox._y + (globals::g_tileHeight / 2)) - (globals::g_screenHeight / 2);

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

void Goomba::doAnimations()
{
	if (m_playerVelocityX != 0.0f)
	{
		if (m_grounded)
		{
			AnimatedSprite::playAnimation("goombaWalk");
		}
		else
		{
			AnimatedSprite::playAnimation("goombaStop");
		}
	}
	else
	{
		AnimatedSprite::playAnimation("goombaStop");
	}
}

void Goomba::setupAnimation()
{
	AnimatedSprite::addAnimation(2, 0, 0, "goombaWalk", 16, 16);
	AnimatedSprite::addAnimation(1, 0, 0, "goombaStop", 16, 16);
}

bool Goomba::getRightHeld()
{
	return m_rightHeld;
}

void Goomba::setRightHeld(const bool rightHeld)
{
	m_rightHeld = rightHeld;
}

bool Goomba::getLeftHeld()
{

	return m_leftHeld;
}

void Goomba::setLefttHeld(const bool leftHeld)
{
	m_leftHeld = leftHeld;
}

bool Goomba::getGrounded()
{
	return m_grounded;
}

void Goomba::setGrounded(const bool grounded)
{
	m_grounded = grounded;
}

float Goomba::getPlayerVelocityX()
{
	return m_playerVelocityX;
}

void Goomba::setPlayerVelocityX(const float xVel)
{
	m_playerVelocityX = xVel;
}
