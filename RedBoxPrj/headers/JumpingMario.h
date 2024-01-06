#pragma once
#include <iostream>
#include "AnimatedSprite.h"
#include "Globals.h"


class Graphics;

class JumpingMario : public AnimatedSprite
{
public:
	JumpingMario(Graphics& graphics, float x, float y);

	void update(const float& elapsedTime, const std::vector<Tile>& collisionTiles);

	void handleCollisions(const std::vector<Tile>& collisionTiles);

	void doAnimations();

	void draw(Graphics& graphics, Rectangle& camera);

	void setupAnimation();


private:

	float	m_playerVelocityX;
	float	m_playerVelocityY;
	float	m_accelX;
	float	m_accelY;
	float	m_accelerationMagX;
	float	m_accelerationMagY;
	bool	m_grounded;

};