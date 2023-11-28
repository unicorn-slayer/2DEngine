#pragma once
#include <SDL.h>
#include <vector>
#include <map>
#include "Rectangle.h"
#include "Level.h"

class Graphics;

class RedBox
{
public:
	RedBox(Graphics& graphics, float x, float y);
	void handleEvent(SDL_Event& e);
	void update(const float& elapsedTime, const std::vector<Tile>& collisionTiles);
	void draw(Graphics& graphics, Rectangle& camera);
	void setCamera(Rectangle& camera);

	// method that parses a spritesheet for a specific animation and stores component frames into a vector of rectangles.
	// this vector of rectangles is then kept inside a map, which can be indexed by the animation name.
	void addAnimation(int frames, int x, int y, std::string name, int width, int height);

	// method that sets a member variable, _current animation to the argument animation. This member variable is used later
	// in the update method, letting it know which animation to update. Also inits frame index to 0.
	void playAnimation(std::string animation);

	// this method updates the frame index for the animation. The animate draw method will now draw the correct frame.This might be merged into existing update
	// function.
	void animationUpdate(float elapsedTime);

	// this method will draw the correct frame at the specified x and y value that the player is add. This might be merged into existing draw
	// function.
	void animateDraw(Graphics& graphics, int x, int y);

	void setupAnimation();


private:
	float _x;
	float _y;
	float _accelX;
	float _accelY;
	float _accelerationMagX;
	float _accelerationMagY;
	int _width;
	int _height;
	bool _jumping;
	bool _grounded;
	int _frameCounter;
	std::map < std::string, std::vector<SDL_Rect>> _animations;
	std::string _currentAnimation;
	int _frameIndex;
	float _timeElapsed;
	float _timeUpdate;
	bool _rightHeld;
	bool _leftHeld;

	float playerVelocityX;
	float playerVelocityY;

	SDL_Rect _destRect;
	SDL_Texture* _redBox;
	SDL_Texture* _spriteSheet;

	Rectangle _rect;
};
