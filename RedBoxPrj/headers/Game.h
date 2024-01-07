#pragma once
#include "RedBox.h"

class Goomba;

class Mario;

class Luigi;

class Fireballs;

class JumpingMario;

class ItemBox;

class Game
{
public:

	Game();

	void gameLoop();

	void handleEvents(std::vector<Goomba>& goombas, Goomba& cameraGoomba, SDL_Event& event);

	void updateEntities(std::vector<Goomba>& goombas, Goomba& cameraGoomba, std::vector<Mario>& marios, std::vector<Luigi>& luigis, std::vector<std::vector<Fireballs>>& fireballsVector, std::vector<JumpingMario>& jumpingMarios, std::vector<ItemBox>& itemBoxes, const std::vector<Tile>& tiles, float& timeStep, Graphics& graphics);

	void goombaFollow(std::vector<Goomba>& goombas);

	void checkCollisions(std::vector<Mario>& marios, std::vector<Luigi>& luigis, std::vector<Goomba>& goombas, std::vector<std::vector<Fireballs>>& fireballsVector, std::vector<JumpingMario>& jumpingMarios, const std::vector<Tile>& tiles);
	
	void animationUpdate(std::vector<Goomba>& goombas, std::vector<Mario>& marios, std::vector<Luigi>& luigis, std::vector<std::vector<Fireballs>>& fireballsVector, std::vector<JumpingMario>& jumpingMarios, std::vector<ItemBox>& itemBoxes, float& timeStep);

	void focusCamera(std::vector<Goomba>& goombas, Goomba& cameraGoomba);

	void drawEntities(std::vector<Goomba>& goombas, std::vector<Mario>& marios, std::vector<Luigi>& luigis, std::vector<std::vector<Fireballs>>& fireballsVector, std::vector<JumpingMario>& jumpingMarios, std::vector<ItemBox>& itemBoxes, Graphics& graphics, Rectangle& camera);


private:

};