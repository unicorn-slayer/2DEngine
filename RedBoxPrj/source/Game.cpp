#include "Game.h"
#include <SDL.h>
#include "Graphics.h"
#include "Input.h"
#include "RedBox.h"
#include "Level.h"
#include "Globals.h"
#include "Goomba.h"
#include <iostream>
#include <algorithm>
#include "Mario.h"

Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	this->gameLoop();
}

void Game::gameLoop()
{
	Graphics graphics;
	Input input;
	Level level;
	level.loadMap(graphics, "map.tmx");
	Rectangle camera = { 0, 0, 700, 700 };


	std::vector<Goomba> goombas;

	for (int i = 0; i < 4; ++i) {
		Goomba goomba(graphics, globals::g_centreX, globals::g_centreY - 300);
		goombas.push_back(goomba);
	}

	Goomba cameraGoomba(graphics, globals::g_centreX, globals::g_centreY - 300);

	Mario mario(graphics, globals::g_centreX - 200, globals::g_centreY - 300);

	SDL_Event event;

	float delayTime = (1000.0f/60.0f);

	float timeStep = 1.0f;


	while (true)
	{
		float startTimer = (float)SDL_GetTicks();

		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				return;
			}

			this->handleEvents(goombas, cameraGoomba, event);

		}

		cameraGoomba.setCamera(camera);

		this->updateCharacters(goombas, cameraGoomba, mario, level.getCollisionTiles(), timeStep);

		this->checkEnemyCollisions(mario, goombas);

		this->animationUpdate(goombas, mario, timeStep);

		graphics.clear();

		level.draw(graphics, camera);

		this->drawCharacters(goombas, mario, graphics, camera);

		graphics.flip();

		float frameTime = SDL_GetTicks() - startTimer;

		if (frameTime < delayTime) {

			SDL_Delay((Uint32)(delayTime - frameTime));

		}

	}

}

void Game::handleEvents(std::vector<Goomba>& goombas, Goomba& cameraGoomba, SDL_Event& event)
{
	if (goombas.size())
	{
		goombas[0].handleEvent(event);
	}

	cameraGoomba.handleEvent(event);
}

void Game::updateCharacters(std::vector<Goomba>& goombas, Goomba& cameraGoomba, Mario& mario, const std::vector<Tile>& tiles, float& timeStep)
{
	mario.update(timeStep, tiles);

	if (goombas.size())
	{
		goombas[0].update(timeStep, tiles);
	}

	cameraGoomba.update(timeStep, tiles);

	if (goombas.size())
	{
		this->goombaFollow(goombas);
	}
}

void Game::goombaFollow(std::vector<Goomba>& goombas)
{
	constexpr int MAX_POSITIONS = 3;

	for (int i = 0; i < goombas.size() - 1; ++i) {
		if (i == goombas.size()-1) {
			continue;
		}

		Goomba& currentGoomba = goombas[i];
		Goomba& nextGoomba = goombas[i + 1];

		std::queue<goombaData>& goombaDataQueue = currentGoomba.m_goombaDataQueue;

		if (goombaDataQueue.size() > MAX_POSITIONS) {

			nextGoomba.setX(goombaDataQueue.front().xPosition);
			nextGoomba.setY(goombaDataQueue.front().yPosition);
			nextGoomba.setGrounded(goombaDataQueue.front().grounded);
			nextGoomba.setPlayerVelocityX(goombaDataQueue.front().xVelocity);
			nextGoomba.setLefttHeld(goombaDataQueue.front().left_held);
			nextGoomba.setRightHeld(goombaDataQueue.front().right_held);

			if (nextGoomba.m_goombaDataQueue.size() < (MAX_POSITIONS + 1))
			{
				nextGoomba.m_goombaDataQueue.emplace(nextGoomba.getX(), nextGoomba.getY(), nextGoomba.getGrounded(), nextGoomba.getPlayerVelocityX(), nextGoomba.getRightHeld(), nextGoomba.getLeftHeld());
			}

			goombaDataQueue.pop();

		}
	}
}

void Game::checkEnemyCollisions(Mario& mario, std::vector<Goomba>& goombas)
{
	if (goombas.size() > 0)
	{
		for (int i = 0; i < goombas.size(); i++)
		{
			if (goombas[i].getBoundingBox().checkCollision(mario.getBoundingBox()))
			{
				goombas.erase(goombas.begin() + i);
			}
		}
	}
}

void Game::animationUpdate(std::vector<Goomba>& goombas, Mario& mario, float& timeStep)
{
	if (goombas.size() > 1)
	{
		for (int i = 1; i < goombas.size(); i++)
		{
			goombas[i].doAnimations();

			goombas[i].animationUpdate(timeStep);
		}
	}

	mario.doAnimations();

	mario.animationUpdate(timeStep);
}

void Game::drawCharacters(std::vector<Goomba>& goombas, Mario& mario, Graphics& graphics, Rectangle& camera)
{
	if (goombas.size())
	{
		for (int i = 0; i < goombas.size(); i++)
		{
			goombas[i].draw(graphics, camera);
		}
	}

	mario.draw(graphics, camera);
}
