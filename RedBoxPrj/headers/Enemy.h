#pragma once
#include <iostream>
#include "AnimatedSprite.h"
#include "Globals.h"

class Fireballs;

class Graphics;

class Enemy : public AnimatedSprite
{
public:
	Enemy(Graphics& graphics, const std::string& filePath, int sourceWidth, int sourceHeight, int sourceX, int sourceY, int width, int height, float x, float y, float timeToUpdate);

	~Enemy();

	virtual void update(const float& elapsedTime, const std::vector<Tile>& collisionTiles);

	virtual void handleCollisions(const std::vector<Tile>& collisionTiles);

	virtual void doAnimations();

	virtual void draw(Graphics& graphics, Rectangle& camera);

	virtual void die();

	bool		m_alive;

protected:

	float		m_playerVelocityX;
	float		m_playerVelocityY;
	float		m_accelX;
	float		m_accelY;
	float		m_accelerationMagX;
	float		m_accelerationMagY;
	bool		m_grounded;
	Graphics	m_graphics;

};

class Mario : public Enemy
{
public:
	Mario(Graphics& graphics, float x, float y);

	void setupAnimation();

private:

};

class Luigi : public Enemy
{
public:
	Luigi(Graphics& graphics, float x, float y, std::vector<Fireballs>& fireballs);

	void setupAnimation();

	void update(const float& elapsedTime, const std::vector<Tile>& collisionTiles) override;

	void doAnimations() override;

	void launchFireball();

	std::vector<Fireballs>& m_fireballs;

private:

	int			m_frameCounter;

};

class JumpingMario : public Enemy
{
public:
	JumpingMario(Graphics& graphics, float x, float y);

	void setupAnimation();

	void update(const float& elapsedTime, const std::vector<Tile>& collisionTiles) override;

	virtual void handleCollisions(const std::vector<Tile>& collisionTiles) override;

	void doAnimations() override;

private:

};
