#pragma once
#include <iostream>
#include "AnimatedSprite.h"
#include "Globals.h"

enum Facing {left = -1, right = 1};

class Graphics;

class Fireballs : public AnimatedSprite
{
public:
	Fireballs(Graphics& graphics, float x, float y, Facing facing);

	void update(const float& elapsedTime, const std::vector<Tile>& collisionTiles);

	void handleCollisions(const std::vector<Tile>& collisionTiles);

	void doAnimations();

	void draw(Graphics& graphics, Rectangle& camera);

	void setupAnimation();



private:

	float	m_playerVelocityX;
	float	m_playerVelocityY;
	float	m_accelerationMagY;

};
