#include "Game.h"
#include <SDL.h>
#include "Graphics.h"
#include "Input.h"
#include "RedBox.h"
#include "Level.h"
#include "Globals.h"

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
	RedBox redBox(graphics, globals::g_centreX, globals::g_centreY-300);

	int startTimer = SDL_GetTicks();

	SDL_Event event;

	while (true)
	{
		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				return;
			}

			redBox.handleEvent(event);
		}

		int endTimer = SDL_GetTicks();
		int elapsedTime = endTimer - startTimer;
		startTimer = endTimer;	

		redBox.update(elapsedTime, level.getCollisionTiles());

		graphics.clear();

		level.draw(graphics);

		redBox.draw(graphics);		

		graphics.flip();
	}

}
