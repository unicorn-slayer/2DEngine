#pragma once
#include <map>
#include <string>
#include <vector>
#include "Sprite.h"
#include "Globals.h"

class Graphics;

class AnimatedSprite : public Sprite
{
public:

	AnimatedSprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height, float x, float y, float timeToUpdate);

	void addAnimation(int frames, int x, int y, std::string name, int width, int height);

	void playAnimation(std::string animation);

	void animationUpdate(float elapsedTime);

	void animateDraw(Graphics& graphics, int x, int y);

protected:
	std::string m_currentAnimation;
	int m_frameIndex;
	float m_timeToUpdate;

private:
	std::map < std::string, std::vector<SDL_Rect>> m_animations;
	float m_timeElapsed;

};