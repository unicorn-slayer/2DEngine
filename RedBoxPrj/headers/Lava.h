#pragma once
#include <iostream>
#include "AnimatedSprite.h"
#include "Globals.h"

class Graphics;

class Lava : public AnimatedSprite
{
public:
	Lava(Graphics& graphics, float x, float y);

	void update(const float& elapsedTime, const std::vector<Tile>& collisionTiles);

	void handleCollisions(const std::vector<Tile>& collisionTiles);

	void doAnimations();

	void draw(Graphics& graphics, Rectangle& camera);

	void setupAnimation();

private:

	float	m_lavaVelocityX;
	float	m_lavaVelocityY;
	float	m_accelX;
	float	m_accelY;
	float	m_accelerationMagX;
	float	m_accelerationMagY;
	int		m_frameCounter;

};
