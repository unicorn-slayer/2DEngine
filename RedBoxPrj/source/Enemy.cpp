#include "Enemy.h"
#include "Fireballs.h"
#include <algorithm>

//put all parameters in Enemy and pass down to AnimatedSprite
Enemy::Enemy(Graphics& graphics, const std::string& filePath, int sourceWidth, int sourceHeight, int sourceX, int sourceY, int width, int height, float x, float y, float timeToUpdate) 
	: AnimatedSprite(graphics, filePath, sourceWidth, sourceHeight, sourceX, sourceY, width, height, x, y, timeToUpdate)
	, m_playerVelocityX(0)
	, m_playerVelocityY(0)
	, m_accelX(0)
	, m_accelY(0)
	, m_accelerationMagX(0.2f)
	, m_accelerationMagY(0.3f)
	, m_graphics(graphics)
	, m_grounded(false)
	, m_alive(true)
{
}

Enemy::~Enemy()
{
}

void Enemy::update(const float& elapsedTime, const std::vector<Tile>& collisionTiles)
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

void Enemy::handleCollisions(const std::vector<Tile>& collisionTiles)
{
	if (m_alive)
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
				Sprite::m_y = tile.boundingBox._y + Sprite::m_height + 1;
				Sprite::m_boundingBox._y = tile.boundingBox._y + Sprite::m_height + 1;
				break;

			case sides::BOTTOM:
				Sprite::m_y = tile.boundingBox._y - Sprite::m_height;
				Sprite::m_boundingBox._y = tile.boundingBox._y - Sprite::m_height;
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
}

void Enemy::doAnimations()
{
	if (m_alive)
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
	else
	{
		AnimatedSprite::playAnimation("miniMarioDie");
	}

}

void Enemy::draw(Graphics& graphics, Rectangle& camera)
{
	SDL_Rect destRect = { (int)Sprite::m_x, (int)Sprite::m_y, Sprite::m_width, Sprite::m_height };

	//when drawing, need to offset the redBox by the camera distance
	destRect.x = destRect.x - (int)camera._x;
	destRect.y = destRect.y - (int)camera._y;

	AnimatedSprite::animateDraw(graphics, (int)destRect.x, (int)destRect.y, destRect.w, destRect.h);
}

void Enemy::die()
{
	m_alive = false;

	m_playerVelocityY = -10.0f;

	Sprite::m_boundingBox._height = 0;
	Sprite::m_boundingBox._width = 0;
	Sprite::m_boundingBox._x = 0;
	Sprite::m_boundingBox._y = 0;


}


Mario::Mario(Graphics& graphics, float x, float y)
	: Enemy(graphics, "goomba.png", 16, 16, 0, 16, 32, 32, x, y, 5)
{
	this->setupAnimation();
	AnimatedSprite::playAnimation("miniMarioRight");
}

void Mario::setupAnimation()
{
	AnimatedSprite::addAnimation(4, 0, 16, "miniMarioRight", 16, 16);
	AnimatedSprite::addAnimation(4, 0, 32, "miniMarioLeft", 16, 16);
	AnimatedSprite::addAnimation(1, 0, 16, "miniMarioStop", 16, 16);
	AnimatedSprite::addAnimation(1, 0, 280, "miniMarioDie", 16, 16);
}





Luigi::Luigi(Graphics& graphics, float x, float y, std::vector<Fireballs>& fireballs)
	: Enemy(graphics, "goomba.png", 16, 32, 0, 48, 32, 64, x, y, 5)
	, m_fireballs(fireballs)
	, m_frameCounter(0)
{
	this->setupAnimation();
	AnimatedSprite::playAnimation("bigLuigiRight");
}

void Luigi::update(const float& elapsedTime, const std::vector<Tile>& collisionTiles)
{
	Enemy::update(elapsedTime, collisionTiles);

	Luigi::launchFireball();
}

void Luigi::doAnimations()
{
	if (m_playerVelocityX != 0)
	{
		if (m_playerVelocityX > 0)
		{
			AnimatedSprite::playAnimation("bigLuigiRight");
		}
		else
		{
			AnimatedSprite::playAnimation("bigLuigiLeft");
		}
	
	}
	else
	{
		AnimatedSprite::playAnimation("bigLuigiRight");
	}
}



void Luigi::launchFireball()
{

	if (m_frameCounter == 10)
	{
		if (Enemy::m_playerVelocityX > 0)
		{
			m_fireballs.push_back(Fireballs(m_graphics, m_x, m_y + 48, right));
		}
		else
		{
			m_fireballs.push_back(Fireballs(m_graphics, m_x, m_y + 48, left));
		}

		m_frameCounter = 0;
	}

	m_frameCounter++;

}

void Luigi::setupAnimation()
{
	AnimatedSprite::addAnimation(3, 0, 48, "bigLuigiRight", Sprite::m_sourceRect.w, Sprite::m_sourceRect.h);
	AnimatedSprite::addAnimation(3, 0, 80, "bigLuigiLeft", Sprite::m_sourceRect.w, Sprite::m_sourceRect.h);
}



JumpingMario::JumpingMario(Graphics& graphics, float x, float y)
	: Enemy(graphics, "goomba.png", 16, 32, 0, 120, 32, 64, x, y, 5)
{
	this->setupAnimation();
	AnimatedSprite::playAnimation("jumpRight");
}

void JumpingMario::update(const float& elapsedTime, const std::vector<Tile>& collisionTiles)
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

	if (m_alive)
	{
		this->handleCollisions(collisionTiles);

	}
}

void JumpingMario::handleCollisions(const std::vector<Tile>& collisionTiles)
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
			Sprite::m_y = tile.boundingBox._y + Sprite::m_height;
			Sprite::m_boundingBox._y = tile.boundingBox._y + Sprite::m_height;
			break;

		case sides::BOTTOM:
			Sprite::m_y = tile.boundingBox._y - Sprite::m_height;
			Sprite::m_boundingBox._y = tile.boundingBox._y - Sprite::m_height;
			m_playerVelocityY = -8.4f;
			m_grounded = true;
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


void JumpingMario::doAnimations()
{
	if (m_grounded)
	{
		AnimatedSprite::playAnimation("Grounded");

		m_grounded = false;
	}
	else
	{
		if (m_playerVelocityX > 0)
		{
			AnimatedSprite::playAnimation("jumpRight");
		}
		else
		{
			AnimatedSprite::playAnimation("jumpLeft");
		}
	}
}

void JumpingMario::setupAnimation()
{
	AnimatedSprite::addAnimation(1, 0, 120, "jumpRight", Sprite::m_sourceRect.w, Sprite::m_sourceRect.h);
	AnimatedSprite::addAnimation(1, 0, 152, "jumpLeft", Sprite::m_sourceRect.w, Sprite::m_sourceRect.h);
	AnimatedSprite::addAnimation(1, 0, 184, "Grounded", Sprite::m_sourceRect.w, Sprite::m_sourceRect.h);
}
