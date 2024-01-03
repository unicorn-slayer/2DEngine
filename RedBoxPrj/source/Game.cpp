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
#include "JumpingMario.h"

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

	//std::vector<Fireballs> fireballs;
	std::vector<std::vector<Fireballs>> fireballsVector;

	std::vector<std::pair<int, int>> spawn = level.getMarioSpawnPoints();

	std::vector<Mario> marios;

	for (int i = (int)spawn.size()-1; i >= 0; i--)
	{
		int x = spawn.back().first;
		int y = spawn.back().second;

		Mario mario(graphics, (float)x, (float)y);
		marios.push_back(mario);

		spawn.pop_back();
	} /////////////////

	spawn = level.getLuigiSpawnPoints();

	std::vector<Luigi> luigis;

	std::vector<Fireballs> fires = {};

	for (int i = 0; i < spawn.size(); i++)
	{
		fireballsVector.push_back(fires);
	}

	for (int i = (int)spawn.size() - 1; i >= 0; i--)
	{

		int x = spawn.back().first;
		int y = spawn.back().second;

		Luigi luigi(graphics, (float)x, (float)y, fireballsVector[i]);
		luigis.push_back(luigi);

		spawn.pop_back();
	}


	//Luigi luigi(graphics, globals::g_centreX - 300, globals::g_centreY - 300, fireballs);
	JumpingMario jumpingMario(graphics, globals::g_centreX - 400, globals::g_centreY - 300);



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

		this->updateEntities(goombas, cameraGoomba, marios, luigis, fireballsVector, jumpingMario, level.getCollisionTiles(), timeStep, graphics);

		this->checkCollisions(marios, luigis, goombas, fireballsVector, jumpingMario, level.getCollisionTiles());

		this->animationUpdate(goombas, marios, luigis, fireballsVector, jumpingMario, timeStep);

		this->focusCamera(goombas, cameraGoomba);

		graphics.clear();

		level.draw(graphics, camera);

		this->drawEntities(goombas, marios, luigis, fireballsVector, jumpingMario, graphics, camera);

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

void Game::updateEntities(std::vector<Goomba>& goombas, Goomba& cameraGoomba, std::vector<Mario>& marios, std::vector<Luigi>& luigis, std::vector<std::vector<Fireballs>>& fireballsVector, JumpingMario& jumpingMario, const std::vector<Tile>& tiles, float& timeStep, Graphics& graphics)
{
	if (marios.size())
	{
		for (int i = 0; i < marios.size(); i++)
		{
			marios[i].update(timeStep, tiles);
		}
	}

	if (luigis.size())
	{
		for (int i = 0; i < luigis.size(); i++)
		{
			luigis[i].update(timeStep, tiles, graphics);
		}
	}

	//luigi.update(timeStep, tiles, graphics);

	jumpingMario.update(timeStep, tiles, graphics);


	if (goombas.size())
	{
		goombas[0].update(timeStep, tiles);
	}

	cameraGoomba.update(timeStep, tiles);

	if (goombas.size())
	{
		this->goombaFollow(goombas);
	}

	if (fireballsVector.size())
	{
		for (int i = 0; i < fireballsVector.size(); i++)
		{
			if (fireballsVector[i].size())
			{
				for (int j = 0; j < fireballsVector[i].size(); j++)
				{
					fireballsVector[i][j].update(timeStep, tiles);
				}				
			}
		}
	}

	//if (fireballs.size())
	//{
	//	for (int i = 0; i < fireballs.size(); i++)
	//	{
	//		fireballs[i].update(timeStep, tiles);
	//	}
	//}
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

void Game::checkCollisions(std::vector<Mario>& marios, std::vector<Luigi>& luigis, std::vector<Goomba>& goombas, std::vector<std::vector<Fireballs>>& fireballsVector, JumpingMario& jumpingMario, const std::vector<Tile>& tiles)
{
	if (goombas.size() > 0)
	{
		for (int i = (int)goombas.size()-1; i >=0; i--)
		{
			if (marios.size())
			{
				for (int j = 0; j < marios.size(); j++)
				{

					if (goombas[i].getBoundingBox().checkCollision(marios[j].getBoundingBox()))
					{
						if (goombas.size())
						{
							goombas.erase(goombas.begin() + i);
							break;
						}
					}
					
				}
			}
		}

		//for (int i = 0; i < goombas.size(); i++)
		//{
		//	if (goombas[i].getBoundingBox().checkCollision(luigi.getBoundingBox()))
		//	{
		//		goombas.erase(goombas.begin() + i);
		//	}
		//}

		for (int i = (int)goombas.size() - 1; i >= 0; i--)
		{
			if (luigis.size())
			{
				for (int j = 0; j < luigis.size(); j++)
				{

					if (goombas[i].getBoundingBox().checkCollision(luigis[j].getBoundingBox()))
					{
						if (goombas.size())
						{
							goombas.erase(goombas.begin() + i);
							break;
						}
					}

				}
			}
		}

		//for (int i = 0; i < goombas.size(); i++)
		//{
		//	if (fireballs.size())
		//	{
		//		for (int j = (int)fireballs.size() - 1; j >= 0; j--)
		//		{
		//			if (goombas[i].getBoundingBox().checkCollision(fireballs[j].getBoundingBox()))
		//			{
		//				goombas.erase(goombas.begin() + i);

		//				fireballs.erase(fireballs.begin() + j);
		//				break;
		//			}
		//		}
		//	}
		//}

		startagain:

		if (goombas.size())
		{
			for (int i = 0; i < goombas.size(); i++)
			{
				if (fireballsVector.size())
				{
					for (int j = (int)fireballsVector.size() - 1; j >= 0; j--)
					{
						if (fireballsVector[j].size())
						{
							if (goombas.size())
							{
								for (int k = fireballsVector[j].size() - 1; k >= 0; k--)
								{
									if (goombas[i].getBoundingBox().checkCollision(fireballsVector[j][k].getBoundingBox()))
									{
										goombas.erase(goombas.begin() + i);

										fireballsVector[j].erase(fireballsVector[j].begin() + k);
										//break;

										goto startagain;
									}
								}
							}
						}
					}
				}
			}
		}


	}

	//if (fireballs.size())
	//{
	//	// reverse iteration due to modifying a vector whilest indexing through it.
	//	for (int i = (int)fireballs.size() - 1; i >= 0; i--)
	//	{
	//		for (int j = 0; j < tiles.size(); j++)
	//		{
	//			if (fireballs[i].getBoundingBox().checkCollision(tiles[j].boundingBox))
	//			{
	//				if (fireballs[i].getBoundingBox().getCollisionSide(tiles[j].boundingBox) == sides::LEFT || fireballs[i].getBoundingBox().getCollisionSide(tiles[j].boundingBox) == sides::RIGHT)
	//				{
	//					fireballs.erase(fireballs.begin() + i);
	//					break;
	//				}

	//			}
	//		}
	//	}
	//}

	if (fireballsVector.size())
	{
		// reverse iteration due to modifying a vector whilest indexing through it.
		for (int i = (int)fireballsVector.size() - 1; i >= 0; i--)
		{
			if (fireballsVector[i].size())
			{
				for (int j = fireballsVector[i].size() - 1; j >= 0; j--)
				{
					for (int k = 0; k < tiles.size(); k++)
					{
						if (fireballsVector[i][j].getBoundingBox().checkCollision(tiles[k].boundingBox))
						{
							if (fireballsVector[i][j].getBoundingBox().getCollisionSide(tiles[k].boundingBox) == sides::LEFT || fireballsVector[i][j].getBoundingBox().getCollisionSide(tiles[k].boundingBox) == sides::RIGHT)
							{
								fireballsVector[i].erase(fireballsVector[i].begin() + j);
								break;
							}

						}
					}
				}
			}

		}
	}
}

void Game::animationUpdate(std::vector<Goomba>& goombas, std::vector<Mario>& marios, std::vector<Luigi>& luigis, std::vector<std::vector<Fireballs>>& fireballsVector, JumpingMario& jumpingMario, float& timeStep)
{
	if (goombas.size() > 1)
	{
		for (int i = 1; i < goombas.size(); i++)
		{
			goombas[i].doAnimations();

			goombas[i].animationUpdate(timeStep);
		}
	}

	if (marios.size())
	{
		for (int i = 0; i < marios.size(); i++)
		{
			marios[i].doAnimations();

			marios[i].animationUpdate(timeStep);
		}
	}

	if (luigis.size())
	{
		for (int i = 0; i < luigis.size(); i++)
		{
			luigis[i].doAnimations();

			luigis[i].animationUpdate(timeStep);
		}
	}

	jumpingMario.doAnimations();

	jumpingMario.animationUpdate(timeStep);

	if (fireballsVector.size())
	{
		for (int i = 0; i < fireballsVector.size(); i++)
		{
			if (fireballsVector[i].size())
			{
				for (int j = 0; j < fireballsVector[i].size(); j++)
				{
					fireballsVector[i][j].doAnimations();

					fireballsVector[i][j].animationUpdate(timeStep);
				}

			}
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

void Game::drawEntities(std::vector<Goomba>& goombas, std::vector<Mario>& marios, std::vector<Luigi>& luigis, std::vector<std::vector<Fireballs>>& fireballsVector, JumpingMario& jumpingMario, Graphics& graphics, Rectangle& camera)
{
	if (goombas.size())
	{
		for (int i = 0; i < goombas.size(); i++)
		{
			goombas[i].draw(graphics, camera);
		}
	}

	if (marios.size())
	{
		for (int i = 0; i < marios.size(); i++)
		{
			marios[i].draw(graphics, camera);
		}
	}

	if (luigis.size())
	{
		for (int i = 0; i < luigis.size(); i++)
		{
			luigis[i].draw(graphics, camera);
		}
	}


	jumpingMario.draw(graphics, camera);

	if (fireballsVector.size())
	{
		for (int i = 0; i < fireballsVector.size(); i++)
		{
			if (fireballsVector[i].size())
			{
				for (int j = 0; j < fireballsVector[i].size(); j++)
				{
					fireballsVector[i][j].draw(graphics, camera);

				}
			}
		}
	}
}
