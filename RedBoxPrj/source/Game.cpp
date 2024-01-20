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
#include "fireballs.h"
#include "ItemBox.h"
#include "Lava.h"
#include "Enemy.h"
#include <memory>

Game::Game()
	: m_graphics()
	, m_input()
	, m_level()
	, m_camera({ 0, 0, 700, 700 })
	, m_cameraGoomba(Goomba(m_graphics, globals::g_centreX, globals::g_centreY - 300))
{
	SDL_Init(SDL_INIT_EVERYTHING);
	this->loadResources();
	this->gameLoop();
}

void Game::loadResources()
{
	// load the level
	m_level.loadMap(m_graphics, "map.tmx");

	//load the goombas
	for (int i = 0; i < 4; ++i) {
		Goomba goomba(m_graphics, globals::g_centreX, globals::g_centreY - 300);
		m_goombas.push_back(goomba);
	}



	/// load the marios and add them to enemy list
	std::vector<std::pair<int, int>> spawn = m_level.getMarioSpawnPoints();

	std::vector<std::shared_ptr<Enemy>> marios;

	for (int i = (int)spawn.size() - 8; i >= 0; i--)
	{
		int x = spawn.back().first;
		int y = spawn.back().second;

		marios.push_back(std::make_shared<Mario>(m_graphics, (float)x, (float)y));

		spawn.pop_back();
	}

	m_enemiesList.push_back(marios);



	/// prime the fireballs so we can pass them to each luigi.
	std::vector<Fireballs> fires = {};

	for (int i = 0; i < spawn.size(); i++)
	{
		//m_fireballsVector.push_back(fires);
	}

	/// load the luigis and add them to enemy list along with a fireball vector from above.
	spawn = m_level.getLuigiSpawnPoints();

	std::vector<std::shared_ptr<Enemy>> luigis;
	
	for (int i = (int)spawn.size() - 1; i >= 0; i--)
	{

		//int x = spawn.back().first;
		//int y = spawn.back().second;

		//luigis.push_back(std::make_shared<Luigi>(m_graphics, (float)x, (float)y, m_fireballsVector[i]));

		//spawn.pop_back();
	}

	m_enemiesList.push_back(luigis);


	/// now load the jumpingMarios and add them to enemy list
	spawn = m_level.getJumpingMarioSpawnPoints();

	std::vector<std::shared_ptr<Enemy>> jumpingMarios;

	for (int i = (int)spawn.size() - 1; i >= 0; i--)
	{
		//int x = spawn.back().first;
		//int y = spawn.back().second;

		//jumpingMarios.push_back(std::make_shared<JumpingMario>(m_graphics, (float)x, (float)y));

		//spawn.pop_back();
	}

	m_enemiesList.push_back(jumpingMarios);


	/// load itemboxes then add them to m_itemBoxes
	std::vector<std::pair<int, int>> itemBoxPositions;

	itemBoxPositions = m_level.getItemBoxes();

	for (int i = (int)itemBoxPositions.size() - 1; i >= 0; i--)
	{
		int x = itemBoxPositions.back().first;
		int y = itemBoxPositions.back().second - 2;

		ItemBox itemBox(m_graphics, (float)x, (float)y); // 
		m_itemBoxes.push_back(itemBox); // 

		itemBoxPositions.pop_back();
	}


	/// load lavaPositions then add them to m_lavaVec
	std::vector<std::pair<int, int>> lavaPositions;

	lavaPositions = m_level.getLavaPositions();

	for (int i = (int)lavaPositions.size() - 1; i >= 0; i--)
	{
		int x = lavaPositions.back().first;
		int y = lavaPositions.back().second;

		Lava lava(m_graphics, (float)x, (float)y); // 
		m_lavaVec.push_back(lava); // 

		lavaPositions.pop_back();
	}
}

void Game::gameLoop()
{

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

			this->handleEvents(m_goombas, m_cameraGoomba, event);

		}

		// we focus the main camera on an invisible goomba which spawns wherever the leading goomba spawns. This leading goomba, aka cameraGoomba, will
		// have identical movements to the leading goomba. This ensures the camera only tracks one goomba, and doesn't jump erratically to other
		// goombas if they get killed.
		m_cameraGoomba.setCamera(m_camera);

		this->updateEntities(m_enemiesList, m_goombas, m_cameraGoomba, m_fireballsVector, m_itemBoxes, m_lavaVec, m_level.getCollisionTiles(), timeStep, m_graphics);

		this->checkCollisions(m_enemiesList, m_goombas, m_fireballsVector, m_lavaVec, m_level.getCollisionTiles());

		this->animationUpdate(m_enemiesList, m_goombas, m_fireballsVector, m_itemBoxes, m_lavaVec, timeStep);

		this->focusCamera(m_goombas, m_cameraGoomba);

		// after game logic is processed, this will clear the screen
		m_graphics.clear();


		//these next two lines will draw everything to the back buffer
		m_level.draw(m_graphics, m_camera);

		this->drawEntities(m_enemiesList, m_goombas, m_fireballsVector, m_itemBoxes, m_lavaVec, m_graphics, m_camera);

		// backbuffer becomes frontbuffer, i.e. the display we see
		m_graphics.flip();

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

void Game::updateEntities(std::vector<std::vector<std::shared_ptr<Enemy>>>& enemiesList, std::vector<Goomba>& goombas, Goomba& cameraGoomba, std::vector<std::vector<Fireballs>>& fireballsVector, std::vector<ItemBox>& itemBoxes, std::vector<Lava>& lavaVec, const std::vector<Tile>& tiles, float& timeStep, Graphics& graphics)
{

	if (enemiesList.size())
	{
		for (int i = 0; i < enemiesList.size(); i++)
		{
			if (enemiesList[i].size())
			{
				for (int j = 0; j < enemiesList[i].size(); j++)
				{
					enemiesList[i][j]->update(timeStep, tiles);
				}
			}
		}
	}

	if (itemBoxes.size())
	{
		for (int i = 0; i < itemBoxes.size(); i++)
		{
			itemBoxes[i].update(graphics, goombas);
		}
	}

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

	if (lavaVec.size())
	{
		for (int i = 0; i < lavaVec.size(); i++)
		{
			lavaVec[i].update(timeStep, tiles);
		}
	}

}

void Game::goombaFollow(std::vector<Goomba>& goombas)
{
	constexpr int MAX_POSITIONS = 3;

	for (int i = 0; i <= goombas.size() - 1; i++) {


		if (goombas.size() == 1) {

			Goomba& lastGoomba = goombas[i];
			std::queue<goombaData>& goombaDataQueue = lastGoomba.m_goombaDataQueue;

			if (goombaDataQueue.size() > 3)
			{
				goombaDataQueue.pop();
			}

			continue;
		}

		if (i == goombas.size()-1) {

			//Goomba& lastGoomba = goombas[i];
			//std::queue<goombaData>& goombaDataQueue = lastGoomba.m_goombaDataQueue;

			//if (goombaDataQueue.size() > 3)
			//{
			//	goombaDataQueue.pop();
			//}

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
				
				if (i != goombas.size() - 1)
				{
					nextGoomba.m_goombaDataQueue.emplace(nextGoomba.getX(), nextGoomba.getY(), nextGoomba.getGrounded(), nextGoomba.getPlayerVelocityX(), nextGoomba.getRightHeld(), nextGoomba.getLeftHeld(), nextGoomba.getPlayerAccelX());

				}
			}

			goombaDataQueue.pop();

		}
	}
}

void Game::checkCollisions(std::vector<std::vector<std::shared_ptr<Enemy>>>& enemiesList, std::vector<Goomba>& goombas, std::vector<std::vector<Fireballs>>& fireballsVector, std::vector<Lava>& lavaVec, const std::vector<Tile>& tiles)
{

	startEnemies:
	if (goombas.size() > 0)
	{
		for (int i = (int)goombas.size()-1; i >=0; i--)
		{
			if (enemiesList.size())
			{
				for (int j = 0; j < enemiesList.size(); j++)
				{
					if (enemiesList[j].size())
					{
						for (int k = 0; k < enemiesList[j].size(); k++)
						{
							if (goombas[i].getBoundingBox().checkCollision(enemiesList[j][k]->getBoundingBox()))
							{
								if (goombas.size())
								{
									goombas.erase(goombas.begin() + i);
									goto startEnemies;
								}
							}
						}
					}
				}
			}
		}

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
							for (int k = (int)fireballsVector[j].size() - 1; k >= 0; k--)
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

		for (int i = (int)goombas.size() - 1; i >= 0; i--)
		{
			if (lavaVec.size())
			{
				for (int j = 0; j < lavaVec.size(); j++)
				{

					if (goombas[i].getBoundingBox().checkCollision(lavaVec[j].getBoundingBox()))
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
	}


	if (fireballsVector.size())
	{
		// reverse iteration due to modifying a vector whilest indexing through it.
		for (int i = (int)fireballsVector.size() - 1; i >= 0; i--)
		{
			if (fireballsVector[i].size())
			{
				for (int j = (int)fireballsVector[i].size() - 1; j >= 0; j--)
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

void Game::animationUpdate(std::vector<std::vector<std::shared_ptr<Enemy>>>& enemiesList, std::vector<Goomba>& goombas, std::vector<std::vector<Fireballs>>& fireballsVector, std::vector<ItemBox>& itemBoxes, std::vector<Lava>& lavaVec, float& timeStep)
{

	if (enemiesList.size())
	{
		for (int i = 0; i < enemiesList.size(); i++)
		{
			if (enemiesList[i].size())
			{
				for (int j = 0; j < enemiesList[i].size(); j++)
				{
					enemiesList[i][j]->doAnimations();

					enemiesList[i][j]->animationUpdate(timeStep);
				}
			}
		}
	}

	if (goombas.size() > 1)
	{
		for (int i = 1; i < goombas.size(); i++)
		{
			goombas[i].doAnimations();

			goombas[i].animationUpdate(timeStep);
		}
	}

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

	if (itemBoxes.size())
	{
		for (int i = 0; i < itemBoxes.size(); i++)
		{
			itemBoxes[i].doAnimations();

			itemBoxes[i].animationUpdate(timeStep);
		}
	}

	if (lavaVec.size())
	{
		for (int i = 0; i < lavaVec.size(); i++)
		{
			lavaVec[i].doAnimations();

			lavaVec[i].animationUpdate(timeStep);
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

void Game::drawEntities(std::vector<std::vector<std::shared_ptr<Enemy>>>& enemiesList, std::vector<Goomba>& goombas, std::vector<std::vector<Fireballs>>& fireballsVector, std::vector<ItemBox>& itemBoxes, std::vector<Lava>& lavaVec, Graphics& graphics, Rectangle& camera)
{

	if (enemiesList.size())
	{
		for (int i = 0; i < enemiesList.size(); i++)
		{
			if (enemiesList[i].size())
			{
				for (int j = 0; j < enemiesList[i].size(); j++)
				{
					enemiesList[i][j]->draw(graphics, camera);

				}
			}
		}
	}

	if (goombas.size())
	{
		for (int i = 0; i < goombas.size(); i++)
		{
			goombas[i].draw(graphics, camera);
		}
	}

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

	if (itemBoxes.size())
	{
		for (int i = 0; i < itemBoxes.size(); i++)
		{
			itemBoxes[i].draw(graphics, camera);
		}
	}

	if (lavaVec.size())
	{
		for (int i = 0; i < lavaVec.size(); i++)
		{
			lavaVec[i].draw(graphics, camera);
		}
	}
}
