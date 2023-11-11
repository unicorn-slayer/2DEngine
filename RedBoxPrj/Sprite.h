#pragma once
#include <SDL.h>
#include <vector>
#include <map>
#include "Rectangle.h"
#include "Level.h"

class Graphics;

class Sprite
{
public:
	Sprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height, float x, float y);
	void update();
	void draw(Graphics& graphics, int x, int y);

protected:
	SDL_Rect m_sourceRect;
	SDL_Texture* m_spriteSheet;
	Rectangle m_boundingBox;
	float m_x, m_y;

private:

};