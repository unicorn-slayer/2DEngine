#include "Lava.h"
#include <algorithm>

Lava::Lava(Graphics& graphics, float x, float y)
	: AnimatedSprite(graphics, "goomba.png", 16, 16, 0, 16, 32, 32, x, y, 5)
	, m_lavaVelocityX(0)
	, m_lavaVelocityY(-20.0f)
	, m_accelerationMagX(0.2f)
	, m_accelerationMagY(0.3f)
	, m_frameCounter(0)
{
	this->setupAnimation();
	AnimatedSprite::playAnimation("lavaUp");
}

void Lava::update(const float& elapsedTime, const std::vector<Tile>& collisionTiles)
{



	// Update y position
	Sprite::m_y += (m_lavaVelocityY * elapsedTime);
	Sprite::m_boundingBox._y += (m_lavaVelocityY * elapsedTime);

	// apply gravity
	if (m_lavaVelocityY < globals::g_gravityCap)
	{
		m_lavaVelocityY += m_accelerationMagY * elapsedTime;
	}



	if (Sprite::getY() > globals::g_screenHeight + 32)
	{
		m_lavaVelocityY = 0;
		m_frameCounter++;

		if(m_frameCounter > 75)
		{
			m_lavaVelocityY = -20.0f;
			m_frameCounter = 0;
		}
	}

}

void Lava::handleCollisions(const std::vector<Tile>& collisionTiles)
{


}

void Lava::doAnimations()
{
	if (m_lavaVelocityY < 0)
	{
		AnimatedSprite::playAnimation("lavaUp");
	}
	else
	{
		AnimatedSprite::playAnimation("lavaDown");
	}
}

void Lava::draw(Graphics& graphics, Rectangle& camera)
{
	SDL_Rect destRect = { (int)Sprite::m_x, (int)Sprite::m_y, Sprite::m_width, Sprite::m_height };

	//when drawing, need to offset the redBox by the camera distance
	destRect.x = destRect.x - (int)camera._x;
	destRect.y = destRect.y - (int)camera._y;

	AnimatedSprite::animateDraw(graphics, (int)destRect.x, (int)destRect.y, destRect.w, destRect.h);
}

void Lava::setupAnimation()
{
	AnimatedSprite::addAnimation(1, 0,248, "lavaUp", 16, 16);
	AnimatedSprite::addAnimation(1, 0, 264, "lavaDown", 16, 16);
}
