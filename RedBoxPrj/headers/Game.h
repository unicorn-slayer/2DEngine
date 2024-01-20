#pragma once
#include <memory>
#include <vector>
#include <SDL.h>
#include <map>
#include "Graphics.h"
#include "Input.h"
#include "Level.h"
#include "Goomba.h"
#include "Enemy.h"
#include "Lava.h"
#include "ItemBox.h"
#include "Fireballs.h"

struct Tile;

class Rectangle;

class Mario;

class Luigi;

class Fireballs;

class JumpingMario;

class ItemBox;

class Lava;

class Enemy;


class Game
{
public:

	Game();

	void loadResources();

	void gameLoop();

	void handleEvents(std::vector<Goomba>& goombas, Goomba& cameraGoomba, SDL_Event& event);

	void updateEntities(std::vector<std::vector<std::shared_ptr<Enemy>>>& enemiesList, std::vector<Goomba>& goombas, Goomba& cameraGoomba, std::vector<std::vector<Fireballs>>& fireballsVector, std::vector<ItemBox>& itemBoxes, std::vector<Lava>& lavaVec, const std::vector<Tile>& tiles, float& timeStep, Graphics& graphics);

	void goombaFollow(std::vector<Goomba>& goombas);

	void checkCollisions(std::vector<std::vector<std::shared_ptr<Enemy>>>& enemiesList, std::vector<Goomba>& goombas, std::vector<std::vector<Fireballs>>& fireballsVector, std::vector<Lava>& lavaVec, const std::vector<Tile>& tiles);
	
	void animationUpdate(std::vector<std::vector<std::shared_ptr<Enemy>>>& enemiesList, std::vector<Goomba>& goombas, std::vector<std::vector<Fireballs>>& fireballsVector,  std::vector<ItemBox>& itemBoxes, std::vector<Lava>& lavaVec, float& timeStep);

	void focusCamera(std::vector<Goomba>& goombas, Goomba& cameraGoomba);

	void drawEntities(std::vector<std::vector<std::shared_ptr<Enemy>>>& enemiesList, std::vector<Goomba>& goombas, std::vector<std::vector<Fireballs>>& fireballsVector, std::vector<ItemBox>& itemBoxes, std::vector<Lava>& lavaVec, Graphics& graphics, Rectangle& camera);


private:

	Graphics												m_graphics;
	Input													m_input;
	Level													m_level;
	Rectangle												m_camera;
	std::vector<Goomba>										m_goombas;
	std::vector<std::vector<Fireballs>>						m_fireballsVector;
	std::vector<std::vector<std::shared_ptr<Enemy>>>		m_enemiesList;
	std::vector<ItemBox>									m_itemBoxes;
	std::vector<Lava>										m_lavaVec;
	Goomba													m_cameraGoomba;
			
};