#pragma once
#include <iostream>
#include "AnimatedSprite.h"
#include "Globals.h"

class Graphics;

class Enemy : public AnimatedSprite
{
public:
	Enemy(Graphics& graphics, const std::string& filePath, int sourceWidth, int sourceHeight, int sourceX, int sourceY, int width, int height, float x, float y, float timeToUpdate);

	virtual void update(const float& elapsedTime, const std::vector<Tile>& collisionTiles);

	virtual void handleCollisions(const std::vector<Tile>& collisionTiles);

	virtual void doAnimations();

	virtual void draw(Graphics& graphics, Rectangle& camera);

private:

	float	m_playerVelocityX;
	float	m_playerVelocityY;
	float	m_accelX;
	float	m_accelY;
	float	m_accelerationMagX;
	float	m_accelerationMagY;

};

class Mario : public Enemy
{
public:
	Mario(Graphics& graphics, float x, float y);

	void setupAnimation();

private:

};
