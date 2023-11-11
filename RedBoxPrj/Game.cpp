#include "Game.h"
#include <SDL.h>
#include "Graphics.h"
#include "Input.h"
#include "RedBox.h"
#include "Level.h"
#include "Globals.h"
#include "Goomba.h"

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
	Goomba goomba(graphics, globals::g_centreX, globals::g_centreY - 300);


	int startTimer = SDL_GetTicks();

	SDL_Event event;

	while (true)
	{
		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				return;
			}

			goomba.handleEvent(event);
		}

		int endTimer = SDL_GetTicks();
		int elapsedTime = endTimer - startTimer;
		startTimer = endTimer;	

		goomba.update((float)elapsedTime, level.getCollisionTiles());
		goomba.setCamera(camera);

		graphics.clear();

		level.draw(graphics, camera);

		goomba.draw(graphics, camera);

		graphics.flip();
	}

}
