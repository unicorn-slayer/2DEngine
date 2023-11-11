#pragma once
#include <iostream>
#include <string>
#include "AnimatedSprite.h"
#include "Globals.h"

class Graphics;

class Goomba : public AnimatedSprite
{
public:
	Goomba(Graphics& graphics, float x, float y);

	void handleEvent(SDL_Event& e);

	void update(const float& elapsedTime, const std::vector<Tile>& collisionTiles);

	void move(const float& elapsedTime);

	void handleCollisions(const std::vector<Tile>& collisionTiles);

	void handleScreenBounds(const float& elapsedTime);

	void draw(Graphics& graphics, Rectangle& camera);

	void setCamera(Rectangle& camera);

	void setupAnimation();

private:
	float	m_accelX;
	float	m_accelY;
	float	m_accelerationMagX;
	float	m_accelerationMagY;
	bool	m_jumping;
	bool	m_grounded;
	int		m_frameCounter;
	bool	m_rightHeld;
	bool	m_leftHeld;
	float	m_playerVelocityX;
	float	m_playerVelocityY;
};
