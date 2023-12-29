#pragma once
#include "RedBox.h"

class Goomba;

class Mario;

class Luigi;

class Fireballs;

class Game
{
public:

	Game();

	void gameLoop();

	void handleEvents(std::vector<Goomba>& goombas, Goomba& cameraGoomba, SDL_Event& event);

	void updateEntities(std::vector<Goomba>& goombas, Goomba& cameraGoomba, Mario& mario, Luigi& luigi, std::vector<Fireballs>& fireballs, const std::vector<Tile>& tiles, float& timeStep, Graphics& graphics);

	void goombaFollow(std::vector<Goomba>& goombas);

	void checkCollisions(Mario& mario, Luigi& luigi, std::vector<Goomba>& goombas, std::vector<Fireballs>& fireballs, const std::vector<Tile>& tiles);
	
	void animationUpdate(std::vector<Goomba>& goombas, Mario& mario, Luigi& luigi, std::vector<Fireballs>& fireballs, float& timeStep);

	void focusCamera(std::vector<Goomba>& goombas, Goomba& cameraGoomba);

	void drawEntities(std::vector<Goomba>& goombas, Mario& mario, Luigi& luigi, std::vector<Fireballs>& fireballs, Graphics& graphics, Rectangle& camera);


private:

};