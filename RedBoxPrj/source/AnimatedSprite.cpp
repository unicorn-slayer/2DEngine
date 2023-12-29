#include "AnimatedSprite.h"


AnimatedSprite::AnimatedSprite(Graphics& graphics, const std::string& filePath, int sourceWidth, int sourceHeight, int sourceX, int sourceY, int width, int height, float x, float y, float timeToUpdate)
	: Sprite(graphics, filePath, sourceWidth, sourceHeight, sourceX, sourceY, width, height, x, y)
	, m_currentAnimation("")
	, m_frameIndex(0)
	, m_timeToUpdate(timeToUpdate)
	, m_animations({})
	, m_timeElapsed(0)
{

}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height)
{
	std::vector<SDL_Rect> rectangles;
	for (int i = 0; i < frames; i++) {
		SDL_Rect newRect = { i * width, y, width, height };
		rectangles.push_back(newRect);
	}
	m_animations.insert(std::pair<std::string, std::vector<SDL_Rect> >(name, rectangles));
}

void AnimatedSprite::playAnimation(std::string animation)
{
	if (m_currentAnimation != animation) {
		m_currentAnimation = animation;
		m_frameIndex = 0;
	}
}

void AnimatedSprite::animationUpdate(float elapsedTime)
{
	Sprite::update();

	m_timeElapsed += elapsedTime;
	if (m_timeElapsed > m_timeToUpdate) {
		m_timeElapsed -= m_timeToUpdate;
		if (m_frameIndex < m_animations[m_currentAnimation].size() - 1) {
			m_frameIndex++;
		}
		else {
			m_frameIndex = 0;
		}
	}
}

void AnimatedSprite::animateDraw(Graphics& graphics, int x, int y, int width, int height)
{
	SDL_Rect destinationRectangle = {};
	destinationRectangle.x = x;
	destinationRectangle.y = y;
	destinationRectangle.w = width;
	destinationRectangle.h = height;

	SDL_Rect sourceRect = m_animations[m_currentAnimation][m_frameIndex];
	graphics.blitSurface(m_spriteSheet, &sourceRect, &destinationRectangle);
}
