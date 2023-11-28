#pragma once
#include "RedBox.h"

class Goomba;

class Game
{
public:

	Game();

	void gameLoop();

	void algorithm(std::vector<Goomba>& goombas);

private:

};