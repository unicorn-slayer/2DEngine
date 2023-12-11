#include "Sprite.h"

Sprite::Sprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height, float x, float y) : m_x(x), m_y(y)
{
	m_sourceRect.x = sourceX;
	m_sourceRect.y = sourceY;
	m_sourceRect.w = width;
	m_sourceRect.x = height;

	m_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
	m_boundingBox = Rectangle(x, y, width, height);
}

void Sprite::update()
{
	m_boundingBox._x = m_x;
	m_boundingBox._y = m_y;
}

void Sprite::draw(Graphics& graphics, int x, int y)
{
	SDL_Rect destinationRectangle = { x, y, m_sourceRect.w, m_sourceRect.h};
	graphics.blitSurface(m_spriteSheet, &m_sourceRect, &destinationRectangle);

}

float Sprite::getX()
{
	return m_x;
}

float Sprite::getY()
{
	return m_y;
}

void Sprite::setX(float x)
{
	m_x = x;
}

void Sprite::setY(float y)
{
	m_y = y;
}

Rectangle Sprite::getBoundingBox()
{
	return m_boundingBox;
}
