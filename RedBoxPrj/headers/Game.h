#pragma once
#include "RedBox.h"

class Goomba;

class Mario;

class Game
{
public:

	Game();

	void gameLoop();

	void handleEvents(std::vector<Goomba>& goombas, Goomba& cameraGoomba, SDL_Event& event);

	void updateCharacters(std::vector<Goomba>& goombas, Goomba& cameraGoomba, Mario& mario, const std::vector<Tile>& tiles, float& timeStep);

	void goombaFollow(std::vector<Goomba>& goombas);

	void checkEnemyCollisions(Mario& mario, std::vector<Goomba>& goombas);

	void animationUpdate(std::vector<Goomba>& goombas, Mario& mario, float& timeStep);

	void drawCharacters(std::vector<Goomba>& goombas, Mario& mario, Graphics& graphics, Rectangle& camera);


private:

};