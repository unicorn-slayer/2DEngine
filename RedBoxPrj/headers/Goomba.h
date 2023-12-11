#pragma once
#include <iostream>
#include <string>
#include <queue>
#include "AnimatedSprite.h"
#include "Globals.h"

struct goombaData
{
	goombaData(float x, float y, bool grounded, float xVelocity, bool right_held, bool left_held) : xPosition(x), yPosition(y), grounded(grounded), xVelocity(xVelocity), right_held(right_held), left_held(left_held) {};
	float xPosition;
	float yPosition;
	bool grounded;
	float xVelocity;
	bool right_held;
	bool left_held;
};

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

	void doAnimations();

	void setupAnimation();

	std::queue<float> m_xPositions;
	std::queue<float> m_yPositions;

	std::queue<goombaData> m_goombaDataQueue;

	bool	m_grounded;
	float	m_playerVelocityX;
	float	m_playerVelocityY;
	bool	m_rightHeld;
	bool	m_leftHeld;

private:
	float	m_accelX;
	float	m_accelY;
	float	m_accelerationMagX;
	float	m_accelerationMagY;
	bool	m_jumping;
	int		m_frameCounter;



};
