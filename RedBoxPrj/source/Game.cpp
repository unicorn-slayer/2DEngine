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
	Rectangle camera = { 0, 0, 700, 700 };

	level.loadMap(graphics, "map.tmx");

	std::vector<Goomba> goombas;

	for (int i = 0; i < 4; ++i) {
		Goomba goomba(graphics, globals::g_centreX, globals::g_centreY - 300);
		goombas.push_back(goomba);
	}

	Mario mario(graphics, globals::g_centreX - 200, globals::g_centreY - 300);

	SDL_Event event;

	float DELAY_TIME = 3.0f;

	while (true)
	{
		float startTimer = (float)SDL_GetTicks();

		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				return;
			}

			goombas[0].handleEvent(event);

		}

		float timeStep = 3.0f;

		goombas[0].update(timeStep, level.getCollisionTiles());

		this->algorithm(goombas);

		mario.update(timeStep, level.getCollisionTiles());

		for (int i = 1; i < goombas.size(); i++)
		{
			goombas[i].doAnimations();

			goombas[i].animationUpdate(timeStep);
		}

		mario.doAnimations();

		mario.animationUpdate(timeStep);

		goombas[0].setCamera(camera);

		graphics.clear();

		level.draw(graphics, camera);

		for (int i = 0; i < goombas.size(); i++)
		{
			goombas[i].draw(graphics, camera);
		}

		mario.draw(graphics, camera);

		graphics.flip();

		float frameTime = SDL_GetTicks() - startTimer;

		if (frameTime < DELAY_TIME) {
			SDL_Delay((Uint32)(DELAY_TIME - frameTime));
		}

	}

}

void Game::algorithm(std::vector<Goomba>& goombas)
{
	constexpr int MAX_POSITIONS = 30;

	for (int i = 0; i < goombas.size() - 1; ++i) {
		if (i == 3) {
			continue;
		}

		auto& currentGoomba = goombas[i];
		auto& nextGoomba = goombas[i + 1];

		auto& goombaDataQueue = currentGoomba.m_goombaDataQueue;

		if (goombaDataQueue.size() > MAX_POSITIONS) {

			nextGoomba.setX(goombaDataQueue.front().xPosition);
			nextGoomba.setY(goombaDataQueue.front().yPosition);
			nextGoomba.m_grounded = goombaDataQueue.front().grounded;
			nextGoomba.m_playerVelocityX = goombaDataQueue.front().xVelocity;

			nextGoomba.m_goombaDataQueue.emplace(nextGoomba.getX(), nextGoomba.getY(), nextGoomba.m_grounded, nextGoomba.m_playerVelocityX);
			goombaDataQueue.pop();
		}
	}
}
