#pragma once
#include <iostream>
#include <string>
#include <queue>
#include "AnimatedSprite.h"
#include "Globals.h"

struct goombaData
{
	goombaData(float x, float y, bool grounded, float xVelocity, bool right_held, bool left_held, float accelX) : xPosition(x), yPosition(y), grounded(grounded), xVelocity(xVelocity), right_held(right_held), left_held(left_held), accelX(accelX) {};
	float xPosition;
	float yPosition;
	bool grounded;
	float xVelocity;
	bool right_held;
	bool left_held;
	float accelX;
};

class Graphics;

class Goomba : public AnimatedSprite
{
public:
	Goomba(Graphics& graphics, float x, float y);

	void handleEvent(SDL_Event& e);

	void update(const float& elapsedTime, const std::vector<Tile>& collisionTiles);

	void move(const float& elapsedTime);

	void updateGoombaDataQueue();

	void handleCollisions(const std::vector<Tile>& collisionTiles);

	void handleScreenBounds(const float& elapsedTime);

	void draw(Graphics& graphics, Rectangle& camera);

	void setCamera(Rectangle& camera);

	void doAnimations();

	void setupAnimation();

	bool getRightHeld();

	void setRightHeld(const bool rightHeld);

	bool getLeftHeld();

	void setLefttHeld(const bool leftHeld);

	bool getGrounded();

	void setGrounded(const bool grounded);

	float getPlayerVelocityX();

	void setPlayerVelocityX(const float xVel);

	void setPlayerVelocityY(const float yVel);

	float getPlayerAccelX();

	void setPlayerAccelX(const float accelX);


	void die();

	std::queue<goombaData> m_goombaDataQueue;

	bool m_alive;
	bool m_oneUpGoomba;
	int m_track;

private:
	float	m_accelY;
	float	m_accelerationMagX;
	float	m_accelerationMagY;
	bool	m_jumping;
	int		m_frameCounter;
	bool	m_rightHeld;
	bool	m_leftHeld;
	float	m_playerVelocityX;
	float	m_playerVelocityY;
	bool	m_grounded;
	float	m_accelX;
};
