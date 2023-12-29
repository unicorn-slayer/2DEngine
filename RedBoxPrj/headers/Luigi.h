#pragma once
#include <iostream>
#include "AnimatedSprite.h"
#include "Globals.h"

class Fireballs;

class Graphics;

class Luigi : public AnimatedSprite
{
public:
	Luigi(Graphics& graphics, float x, float y, std::vector<Fireballs>& fireballs);

	void update(const float& elapsedTime, const std::vector<Tile>& collisionTiles, Graphics& graphics);

	void handleCollisions(const std::vector<Tile>& collisionTiles);

	void doAnimations();

	void draw(Graphics& graphics, Rectangle& camera);

	void setupAnimation();

	void launchFireball(Graphics& graphics);

	std::vector<Fireballs>& m_fireballs;

private:

	float	m_playerVelocityX;
	float	m_playerVelocityY;
	float	m_accelX;
	float	m_accelY;
	float	m_accelerationMagX;
	float	m_accelerationMagY;
	int		m_frameCounter;

};