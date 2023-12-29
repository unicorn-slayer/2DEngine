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
#include "Luigi.h"
#include "fireballs.h"

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

	std::vector<Fireballs> fireballs;


	Mario mario(graphics, globals::g_centreX - 200, globals::g_centreY - 300);
	Luigi luigi(graphics, globals::g_centreX - 300, globals::g_centreY - 300, fireballs);


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

		this->updateEntities(goombas, cameraGoomba, mario, luigi, fireballs, level.getCollisionTiles(), timeStep, graphics);

		this->checkCollisions(mario, luigi, goombas, fireballs, level.getCollisionTiles());

		this->animationUpdate(goombas, mario, luigi, fireballs, timeStep);

		this->focusCamera(goombas, cameraGoomba);

		graphics.clear();

		level.draw(graphics, camera);

		this->drawEntities(goombas, mario, luigi, fireballs, graphics, camera);

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

void Game::updateEntities(std::vector<Goomba>& goombas, Goomba& cameraGoomba, Mario& mario, Luigi& luigi, std::vector<Fireballs>& fireballs, const std::vector<Tile>& tiles, float& timeStep, Graphics& graphics)
{
	mario.update(timeStep, tiles);

	luigi.update(timeStep, tiles, graphics);

	if (goombas.size())
	{
		goombas[0].update(timeStep, tiles);
	}

	cameraGoomba.update(timeStep, tiles);

	if (goombas.size())
	{
		this->goombaFollow(goombas);
	}

	if (fireballs.size())
	{
		for (int i = 0; i < fireballs.size(); i++)
		{
			fireballs[i].update(timeStep, tiles);
		}
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
			nextGoomba.setPlayerAccelX(goombaDataQueue.front().accelX);

			if (nextGoomba.m_goombaDataQueue.size() < (MAX_POSITIONS + 1))
			{
				nextGoomba.m_goombaDataQueue.emplace(nextGoomba.getX(), nextGoomba.getY(), nextGoomba.getGrounded(), nextGoomba.getPlayerVelocityX(), nextGoomba.getRightHeld(), nextGoomba.getLeftHeld(), nextGoomba.getPlayerAccelX());
			}

			goombaDataQueue.pop();

		}
	}
}

void Game::checkCollisions(Mario& mario, Luigi& luigi, std::vector<Goomba>& goombas, std::vector<Fireballs>& fireballs, const std::vector<Tile>& tiles)
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

		for (int i = 0; i < goombas.size(); i++)
		{
			if (goombas[i].getBoundingBox().checkCollision(luigi.getBoundingBox()))
			{
				goombas.erase(goombas.begin() + i);
			}
		}

		for (int i = 0; i < goombas.size(); i++)
		{
			if (fireballs.size())
			{
				for (int j = (int)fireballs.size() - 1; j >= 0; j--)
				{
					if (goombas[i].getBoundingBox().checkCollision(fireballs[j].getBoundingBox()))
					{
						goombas.erase(goombas.begin() + i);

						fireballs.erase(fireballs.begin() + j);
						break;
					}
				}
			}
		}
	}

	if (fireballs.size())
	{
		// reverse iteration due to modifying a vector whilest indexing through it.
		for (int i = (int)fireballs.size() - 1; i >= 0; i--)
		{
			for (int j = 0; j < tiles.size(); j++)
			{
				if (fireballs[i].getBoundingBox().checkCollision(tiles[j].boundingBox))
				{
					if (fireballs[i].getBoundingBox().getCollisionSide(tiles[j].boundingBox) == sides::LEFT || fireballs[i].getBoundingBox().getCollisionSide(tiles[j].boundingBox) == sides::RIGHT)
					{
						fireballs.erase(fireballs.begin() + i);
						break;
					}

				}
			}
		}
	}
}

void Game::animationUpdate(std::vector<Goomba>& goombas, Mario& mario, Luigi& luigi, std::vector<Fireballs>& fireballs, float& timeStep)
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

	luigi.doAnimations();

	luigi.animationUpdate(timeStep);

	if (fireballs.size())
	{
		for (int i = 0; i < fireballs.size(); i++)
		{
			fireballs[i].doAnimations();

			fireballs[i].animationUpdate(timeStep);
		}
	}

}

void Game::focusCamera(std::vector<Goomba>& goombas, Goomba& cameraGoomba)
{
	if (goombas.size())
	{
		if (cameraGoomba.getX() > goombas[0].getX() + 75)
		{
			cameraGoomba.setX(goombas[0].getX() + 75);
		}

		if (cameraGoomba.getX() < goombas[0].getX() - 75)
		{
			cameraGoomba.setX(goombas[0].getX() - 75);
		}
	}
}

void Game::drawEntities(std::vector<Goomba>& goombas, Mario& mario, Luigi& luigi, std::vector<Fireballs>& fireballs, Graphics& graphics, Rectangle& camera)
{
	if (goombas.size())
	{
		for (int i = 0; i < goombas.size(); i++)
		{
			goombas[i].draw(graphics, camera);
		}
	}

	mario.draw(graphics, camera);

	luigi.draw(graphics, camera);

	if (fireballs.size())
	{
		for (int i = 0; i < fireballs.size(); i++)
		{
			fireballs[i].draw(graphics, camera);
		}
	}

}
