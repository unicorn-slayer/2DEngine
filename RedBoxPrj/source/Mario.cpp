#include "Mario.h"
#include <algorithm>


Mario::Mario(Graphics& graphics, float x, float y)
	: AnimatedSprite(graphics, "goomba.png", 0, 16, 32, 32, x, y, 5)
	, m_playerVelocityX(0)
	, m_playerVelocityY(0)
	, m_accelerationMagX(0.2f)
	, m_accelerationMagY(0.3f)
{
	this->setupAnimation();
	AnimatedSprite::playAnimation("miniMarioStop");
}

void Mario::update(const float& elapsedTime, const std::vector<Tile>& collisionTiles)
{

	float maxSpeed = 5.0f;

	m_playerVelocityX += m_accelerationMagX * elapsedTime;

	m_playerVelocityX = std::clamp(m_playerVelocityX, -maxSpeed, maxSpeed);

	// Update x position
	m_x += m_playerVelocityX * elapsedTime;
	m_boundingBox._x += m_playerVelocityX * elapsedTime;

	// apply gravity
	if (m_playerVelocityY < globals::g_gravityCap)
	{
		m_playerVelocityY += m_accelerationMagY * elapsedTime;
	}

	// Update y position
	m_y += (m_playerVelocityY * elapsedTime);
	m_boundingBox._y += (m_playerVelocityY * elapsedTime);

	this->handleCollisions(collisionTiles);

}

void Mario::handleCollisions(const std::vector<Tile>& collisionTiles)
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
			break;

		case sides::LEFT:
			m_playerVelocityX = -m_playerVelocityX;
			m_accelerationMagX = -m_accelerationMagX;
			break;

		case sides::RIGHT:
			m_playerVelocityX = -m_playerVelocityX;
			m_accelerationMagX = -m_accelerationMagX;
			break;
		}
	}
}

void Mario::doAnimations()
{
	if (m_playerVelocityX != 0)
	{
		if (m_playerVelocityX > 0)
		{
			AnimatedSprite::playAnimation("miniMarioRight");
		}
		else
		{
			AnimatedSprite::playAnimation("miniMarioLeft");
		}

	}
	else
	{
		AnimatedSprite::playAnimation("miniMarioStop");
	}
}

void Mario::draw(Graphics& graphics, Rectangle& camera)
{
	SDL_Rect destRect = { (int)Sprite::m_x, (int)Sprite::m_y, globals::g_tileWidth, globals::g_tileHeight };

	//when drawing, need to offset the redBox by the camera distance
	destRect.x = destRect.x - (int)camera._x;
	destRect.y = destRect.y - (int)camera._y;

	AnimatedSprite::animateDraw(graphics, (int)destRect.x, (int)destRect.y);
}

void Mario::setupAnimation()
{
	AnimatedSprite::addAnimation(4, 0, 16, "miniMarioRight", 16, 16);
	AnimatedSprite::addAnimation(4, 0, 32, "miniMarioLeft", 16, 16);
	AnimatedSprite::addAnimation(1, 0, 16, "miniMarioStop", 16, 16);
}
