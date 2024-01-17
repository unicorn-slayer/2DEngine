//#include "Luigi.h"
//#include <algorithm>
//#include "fireballs.h"
//
//
//Luigi::Luigi(Graphics& graphics, float x, float y, std::vector<Fireballs>& fireballs)
//	: AnimatedSprite(graphics, "goomba.png", 16, 32, 0, 48, 32, 64, x, y, 5)
//	, m_playerVelocityX(0)
//	, m_playerVelocityY(0)
//	, m_accelerationMagX(0.2f)
//	, m_accelerationMagY(0.3f)
//	, m_fireballs(fireballs)
//	, m_frameCounter(0)
//	, m_graphics(graphics)
//
//{
//	this->setupAnimation();
//	AnimatedSprite::playAnimation("bigLuigiRight");
//}
//
//void Luigi::update(const float& elapsedTime, const std::vector<Tile>& collisionTiles)
//{
//
//	float maxSpeed = 5.0f;
//
//	m_playerVelocityX += m_accelerationMagX * elapsedTime;
//
//	m_playerVelocityX = std::clamp(m_playerVelocityX, -maxSpeed, maxSpeed);
//
//	// Update x position
//	m_x += m_playerVelocityX * elapsedTime;
//	m_boundingBox._x += m_playerVelocityX * elapsedTime;
//
//	// apply gravity
//	if (m_playerVelocityY < globals::g_gravityCap)
//	{
//		m_playerVelocityY += m_accelerationMagY * elapsedTime;
//	}
//
//	// Update y position
//	m_y += (m_playerVelocityY * elapsedTime);
//	m_boundingBox._y += (m_playerVelocityY * elapsedTime);
//
//	this->handleCollisions(collisionTiles);
//
//	this->launchFireball(m_graphics);
//
//}
//
//void Luigi::handleCollisions(const std::vector<Tile>& collisionTiles)
//{
//
//	std::vector<Tile> collisionTilesActive;
//
//	// handle collisions
//	for (const Tile& tile : collisionTiles)
//	{
//		if (Sprite::m_boundingBox.checkCollision(tile.boundingBox))
//		{
//			collisionTilesActive.push_back(tile);
//		}
//	}
//
//	for (Tile& tile : collisionTilesActive)
//	{
//		sides::Side side = m_boundingBox.getCollisionSide(tile.boundingBox);
//
//		switch (side)
//		{
//		case sides::TOP:
//			m_playerVelocityY = 0;
//			Sprite::m_y = tile.boundingBox._y + Sprite::m_height;
//			Sprite::m_boundingBox._y = tile.boundingBox._y + Sprite::m_height;
//			break;
//
//		case sides::BOTTOM:
//			Sprite::m_y = tile.boundingBox._y - Sprite::m_height;
//			Sprite::m_boundingBox._y = tile.boundingBox._y - Sprite::m_height;
//			m_playerVelocityY = 0;
//			break;
//
//		case sides::LEFT:
//			m_playerVelocityX = -m_playerVelocityX;
//			m_accelerationMagX = -m_accelerationMagX;
//			break;
//
//		case sides::RIGHT:
//			m_playerVelocityX = -m_playerVelocityX;
//			m_accelerationMagX = -m_accelerationMagX;
//			break;
//		}
//	}
//}
//
//void Luigi::doAnimations()
//{
//	if (m_playerVelocityX != 0)
//	{
//		if (m_playerVelocityX > 0)
//		{
//			AnimatedSprite::playAnimation("bigLuigiRight");
//		}
//		else
//		{
//			AnimatedSprite::playAnimation("bigLuigiLeft");
//		}
//
//	}
//	else
//	{
//		AnimatedSprite::playAnimation("bigLuigiRight");
//	}
//}
//
//void Luigi::draw(Graphics& graphics, Rectangle& camera)
//{
//	SDL_Rect destRect = { (int)Sprite::m_x, (int)Sprite::m_y, Sprite::m_width, Sprite::m_height };
//
//	//when drawing, need to offset the redBox by the camera distance
//	destRect.x = destRect.x - (int)camera._x;
//	destRect.y = destRect.y - (int)camera._y;
//
//	AnimatedSprite::animateDraw(graphics, (int)destRect.x, (int)destRect.y, destRect.w, destRect.h);
//}
//
//void Luigi::setupAnimation()
//{
//	AnimatedSprite::addAnimation(3, 0, 48, "bigLuigiRight", Sprite::m_sourceRect.w, Sprite::m_sourceRect.h);
//	AnimatedSprite::addAnimation(3, 0, 80, "bigLuigiLeft", Sprite::m_sourceRect.w, Sprite::m_sourceRect.h);
//}
//
//void Luigi::launchFireball(Graphics& graphics)
//{
//	if (m_frameCounter == 10)
//	{
//		if (m_playerVelocityX > 0)
//		{
//			m_fireballs.push_back(Fireballs(graphics, m_x, m_y + 48, right));
//		}
//		else
//		{
//			m_fireballs.push_back(Fireballs(graphics, m_x, m_y + 48, left));
//		}
//
//		m_frameCounter = 0;
//	}
//
//	m_frameCounter++;
//}
