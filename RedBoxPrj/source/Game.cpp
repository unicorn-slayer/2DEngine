#pragma once
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
	, m_successBox(0, 0)
	, m_levelSuccess(false)
	, m_frameCounter(0)
	, m_menu(m_graphics, "tileset.png", 32, 32, 32, 0, 700, 700, 0, 0)
	, m_title(m_graphics, "title.png", 542, 142, 0, 0, 542, 142, 100, 300)
	, m_end(m_graphics, "end.png", 542, 142, 0, 0, 542, 142, 100, 300)
	, m_play(m_graphics, "play.png", 386, 51, 0, 0, 386, 51, 0, 0)
	, m_levelComplete(false)
	, m_quit(false)
	, m_dead(false)
	, m_playOnce(true)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	this->loadResources();
	this->gameLoop();
}

void Game::loadResources()
{
	// load the level
	m_level.loadMap(m_graphics, "mapFinal.tmx");

	//load the goombas
	for (int i = 0; i < 4; ++i) {
		Goomba goomba(m_graphics, globals::g_centreX, globals::g_centreY - 300);
		m_goombas.push_back(goomba);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

	MusicPlayer::getInstance().playMusic("music/themeMusic.mid");

	/// load the marios and add them to enemy list
	std::vector<std::pair<int, int>> spawn = m_level.getMarioSpawnPoints();

	std::vector<std::shared_ptr<Enemy>> marios;

	for (int i = (int)spawn.size()-1; i >= 0; i--)
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
		m_fireballsVector.push_back(fires);
	}

	/// load the luigis and add them to enemy list along with a fireball vector from above.
	spawn = m_level.getLuigiSpawnPoints();

	std::vector<std::shared_ptr<Enemy>> luigis;
	
	for (int i = (int)spawn.size() - 1; i >= 5; i--)
	{

		int x = spawn.back().first;
		int y = spawn.back().second;

		luigis.push_back(std::make_shared<Luigi>(m_graphics, (float)x, (float)y, m_fireballsVector[i]));

		spawn.pop_back();
	}

	m_enemiesList.push_back(luigis);


	/// now load the jumpingMarios and add them to enemy list
	spawn = m_level.getJumpingMarioSpawnPoints();

	std::vector<std::shared_ptr<Enemy>> jumpingMarios;

	for (int i = (int)spawn.size() - 1; i >= 6; i--)
	{
		int x = spawn.back().first;
		int y = spawn.back().second;

		jumpingMarios.push_back(std::make_shared<JumpingMario>(m_graphics, (float)x, (float)y));

		spawn.pop_back();
	}

	m_enemiesList.push_back(jumpingMarios);


	/// load itemboxes then add them to m_itemBoxes
	std::vector<std::pair<int, int>> itemBoxPositions;

	itemBoxPositions = m_level.getItemBoxes();

	for (int i = (int)itemBoxPositions.size() - 1; i >= 0; i--)
	{
		int x = itemBoxPositions.back().first;
		int y = itemBoxPositions.back().second;

		ItemBox itemBox(m_graphics, (float)x, (float)y); // 
		m_itemBoxes.push_back(itemBox); // 

		itemBoxPositions.pop_back();
	}

	std::vector<std::pair<int, int>> successBoxPosition;

	successBoxPosition = m_level.getSuccessBox();

	m_successBox.m_boundingBox._x = (float)successBoxPosition[0].first;
	m_successBox.m_boundingBox._y = (float)successBoxPosition[0].second;
	m_successBox.m_boundingBox._width = successBoxPosition[1].first;
	m_successBox.m_boundingBox._height = successBoxPosition[1].second;

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
		m_graphics.clear();
		m_menu.draw(m_graphics, 0, 0, 700, 700);
		m_title.draw(m_graphics, 85, 200, 542, 142);
		m_play.draw(m_graphics, 150, 550, 386, 51);
		m_graphics.flip();

		if (SDL_PollEvent(&event)) {

			if (event.key.keysym.sym == SDLK_RETURN) {
				break;
			}
		}
	}


	while (!m_levelComplete)
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

		this->updateEntities(m_enemiesList, m_goombas, m_cameraGoomba, m_deadGoombas, m_fireballsVector, m_itemBoxes, m_lavaVec, m_level.getCollisionTiles(), timeStep, m_graphics);

		this->checkCollisions(m_enemiesList, m_goombas, m_fireballsVector, m_lavaVec, m_level.getCollisionTiles());

		this->animationUpdate(m_enemiesList, m_goombas, m_deadGoombas, m_fireballsVector, m_itemBoxes, m_lavaVec, timeStep);

		this->focusCamera(m_goombas, m_cameraGoomba);

		// after game logic is processed, this will clear the screen
		m_graphics.clear();


		//these next two lines will draw everything to the back buffer
		m_level.draw(m_graphics, m_camera);

		this->drawEntities(m_enemiesList, m_goombas, m_deadGoombas, m_fireballsVector, m_itemBoxes, m_lavaVec, m_graphics, m_camera);

		// backbuffer becomes frontbuffer, i.e. the display we see
		m_graphics.flip();

		float frameTime = SDL_GetTicks() - startTimer;

		if (frameTime < delayTime) {

			SDL_Delay((Uint32)(delayTime - frameTime));

		}



		if(!m_levelSuccess)
		this->checkLevelSuccess(m_goombas, m_successBox);

		if (m_goombas.empty())
		{
			m_dead = true;

		}

		for (int i = 0; i < m_goombas.size(); i++)
		{
			if (m_goombas[i].getY() > 700)
			{
				if (i == 3)
				{
					m_dead = true;
				}
				continue;
			}
			else
			{
				break;
			}

		}

		if (m_dead)
		{
			if (m_playOnce)
			{
				Mix_HaltMusic();
				MusicPlayer::getInstance().playSound("music/gameOver.wav");
				m_playOnce = false;
			}
		}

		if (m_levelSuccess || m_dead)
		{
			m_frameCounter++;

			if (m_frameCounter > 380)
			{
				m_levelComplete = true;
			}
		}
	}

	while (true)
	{
		m_graphics.clear();
		m_menu.draw(m_graphics, 0, 0, 700, 700);

		m_end.draw(m_graphics, 80, 200, 542, 142);

		m_graphics.flip();

		if (SDL_PollEvent(&event)) {

			if (event.key.keysym.sym == SDLK_RETURN) {
				break;
			}

			if (event.key.keysym.sym == SDLK_q) {
				m_quit = true;
				break;
			}
		}
	}

}

void Game::handleEvents(std::vector<Goomba>& goombas, Goomba& cameraGoomba, SDL_Event& event)
{
	if (goombas.size())
	{
		//DO THIS BUT HAVE A CHECK TO SEE IF THE GOOMBA IS ALIVE OR NOT AND ONLY HANDLE EVENTS IF IT IS
		goombas[0].handleEvent(event);
	}

	cameraGoomba.handleEvent(event);
}

void Game::updateEntities(std::vector<std::vector<std::shared_ptr<Enemy>>>& enemiesList, std::vector<Goomba>& goombas, Goomba& cameraGoomba, std::vector<Goomba>& deadGoombas, std::vector<std::vector<Fireballs>>& fireballsVector, std::vector<ItemBox>& itemBoxes, std::vector<Lava>& lavaVec, const std::vector<Tile>& tiles, float& timeStep, Graphics& graphics)
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

	if (deadGoombas.size())
	{
		for (int i = 0; i < deadGoombas.size(); i++)
		{
			deadGoombas[i].update(timeStep, tiles);
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
	constexpr int MAX_POSITIONS = 8;

	for (int i = 0; i <= goombas.size() - 1; i++) {


		Goomba& currGoomba = goombas[i];
		std::queue<goombaData>& goombaDataQueueCurr = currGoomba.m_goombaDataQueue;

		if (goombaDataQueueCurr.size() > MAX_POSITIONS+1)
		{
			goombaDataQueueCurr.pop();
		}

		if (i == goombas.size() - 1)
		{
			continue;
		}
		

		Goomba& currentGoomba = goombas[i];
		Goomba& nextGoomba = goombas[i + 1];
		std::queue<goombaData>& goombaDataQueue = currentGoomba.m_goombaDataQueue;



		if (goombaDataQueue.size() > MAX_POSITIONS) 
			
		{

			nextGoomba.setX(goombaDataQueue.front().xPosition);
			nextGoomba.setY(goombaDataQueue.front().yPosition);
			nextGoomba.setGrounded(goombaDataQueue.front().grounded);
			nextGoomba.setPlayerVelocityX(goombaDataQueue.front().xVelocity);
			nextGoomba.setLefttHeld(goombaDataQueue.front().left_held);
			nextGoomba.setRightHeld(goombaDataQueue.front().right_held);
			nextGoomba.setPlayerAccelX(goombaDataQueue.front().accelX);

			goombaDataQueue.pop();

		}
		

		if (nextGoomba.m_goombaDataQueue.size() < (MAX_POSITIONS + 1))
		{

			if (i != goombas.size() - 1)
			{

				nextGoomba.m_goombaDataQueue.emplace(nextGoomba.getX(), nextGoomba.getY(), nextGoomba.getGrounded(), nextGoomba.getPlayerVelocityX(), nextGoomba.getRightHeld(), nextGoomba.getLeftHeld(), nextGoomba.getPlayerAccelX());
				

			}
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
								if (!goombas[i].getGrounded())
								{
									enemiesList[j][k]->die();
									MusicPlayer::getInstance().playSound("music/enemyDie.wav");

								}
								else
								{
									if (goombas.size())
									{
										if (enemiesList[j][k]->m_alive)
										{

											if (goombas[i].m_visible == true)
											{
												//when goombas die, they turn invisible. However, for the purposes of a death animation, a temporary goombas is created which will do a death animation.
												Goomba goomba(m_graphics, goombas[i].getX(), goombas[i].getY());
												goomba.m_alive = false;
												goomba.setPlayerVelocityY(-5.0f);
												m_deadGoombas.push_back(goomba);

												// the actual goomba dies here
												goombas[i].m_visible = false;
												goombas[i].m_alive = false;

												MusicPlayer::getInstance().playSound("music/goombaDie.wav");

											}

											if (i == 0)
											{
												// if the leading goomba dies, it needs to be erased so the next leading goomba is controlled via the keys.
												goombas.erase(goombas.begin() + i);
												MusicPlayer::getInstance().playSound("music/goombaDie.wav");
												goto startEnemies;

											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		//ISSUE: if goombas hit fireballs then they need their alive status set to false. Same for lava below
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
									//goombas.erase(goombas.begin() + i);



									//fireballsVector[j].erase(fireballsVector[j].begin() + k);
									////break;

									//goto startagain;

									if (goombas[i].m_visible == true)
									{
										//when goombas die, they turn invisible. However, for the purposes of a death animation, a temporary goombas is created which will do a death animation.
										Goomba goomba(m_graphics, goombas[i].getX(), goombas[i].getY());
										goomba.m_alive = false;
										goomba.setPlayerVelocityY(-5.0f);
										m_deadGoombas.push_back(goomba);

										// the actual goomba dies here
										goombas[i].m_visible = false;
										goombas[i].m_alive = false;

										MusicPlayer::getInstance().playSound("music/goombaDie.wav");

									}

									if (i == 0)
									{
										// if the leading goomba dies, it needs to be erased so the next leading goomba is controlled via the keys.
										goombas.erase(goombas.begin() + i);
										MusicPlayer::getInstance().playSound("music/goombaDie.wav");
										goto startEnemies;

									}
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

void Game::animationUpdate(std::vector<std::vector<std::shared_ptr<Enemy>>>& enemiesList, std::vector<Goomba>& goombas, std::vector<Goomba>& deadGoombas, std::vector<std::vector<Fireballs>>& fireballsVector, std::vector<ItemBox>& itemBoxes, std::vector<Lava>& lavaVec, float& timeStep)
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

	if (deadGoombas.size())
	{
		for (int i = 0; i < deadGoombas.size(); i++)
		{
			deadGoombas[i].doAnimations();

			deadGoombas[i].animationUpdate(timeStep);
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
		if (cameraGoomba.getX() > goombas[0].getX() + 350)
		{
			cameraGoomba.setX(goombas[0].getX() + 350);
		}

		if (cameraGoomba.getX() < goombas[0].getX() - 350)
		{
			cameraGoomba.setX(goombas[0].getX() - 350);
		}
	}
}

void Game::drawEntities(std::vector<std::vector<std::shared_ptr<Enemy>>>& enemiesList, std::vector<Goomba>& goombas, std::vector<Goomba>& deadGoombas, std::vector<std::vector<Fireballs>>& fireballsVector, std::vector<ItemBox>& itemBoxes, std::vector<Lava>& lavaVec, Graphics& graphics, Rectangle& camera)
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

	if (deadGoombas.size())
	{
		for (int i = 0; i < deadGoombas.size(); i++)
		{
			deadGoombas[i].draw(graphics, camera);
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

void Game::checkLevelSuccess(std::vector<Goomba>& goombas, SuccessBox& successBox)
{
	m_levelSuccess = successBox.update(goombas);
}
