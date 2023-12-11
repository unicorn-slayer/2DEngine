#pragma once
#include "RedBox.h"

class Goomba;

class Mario;

class Game
{
public:

	Game();

	void gameLoop();

	void algorithm(std::vector<Goomba>& goombas);

	void checkEnemyCollisions(Mario& mario, std::vector<Goomba>& goombas);

private:

};