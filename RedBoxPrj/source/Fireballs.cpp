#include "Fireballs.h"
#include <algorithm>


Fireballs::Fireballs(Graphics& graphics, float x, float y, Facing facing)
	: AnimatedSprite(graphics, "goomba.png", 8, 8, 0, 112, 16, 16, x, y, 5)
	, m_playerVelocityX(8.0f * facing)
	, m_playerVelocityY(0)
	, m_accelerationMagY(0.3f)
{
	this->setupAnimation();
	AnimatedSprite::playAnimation("fireball");
}

void Fireballs::update(const float& elapsedTime, const std::vector<Tile>& collisionTiles)
{

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

void Fireballs::handleCollisions(const std::vector<Tile>& collisionTiles)
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

			break;

		case sides::BOTTOM:
			m_playerVelocityY = -3.4f;

			break;

		case sides::LEFT:
			break;

		case sides::RIGHT:
			break;
		}
	}
}

void Fireballs::doAnimations()
{

	AnimatedSprite::playAnimation("fireball");
}

void Fireballs::draw(Graphics& graphics, Rectangle& camera)
{
	SDL_Rect destRect = { (int)Sprite::m_x, (int)Sprite::m_y, Sprite::m_width, Sprite::m_height };

	//when drawing, need to offset the redBox by the camera distance
	destRect.x = destRect.x - (int)camera._x;
	destRect.y = destRect.y - (int)camera._y;

	AnimatedSprite::animateDraw(graphics, (int)destRect.x, (int)destRect.y, destRect.w, destRect.h);
}

void Fireballs::setupAnimation()
{
	AnimatedSprite::addAnimation(4, 0, 112, "fireball", 8, 8);
}
