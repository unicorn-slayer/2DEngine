#include "ItemBox.h"
#include "Goomba.h"
#include <algorithm>

ItemBox::ItemBox(Graphics& graphics, float x, float y)
	: AnimatedSprite(graphics, "goomba.png", 16, 16, 0, 216, 32, 32, x, y, 15)
	, m_boxEmpty(false)
{
	this->setupAnimation();
	AnimatedSprite::playAnimation("flashingBox");

}

void ItemBox::update(std::vector<Goomba>& goombas)
{
	std::vector<Goomba> collisionGoombas;

	// handle collisions
	for (int i = 0; i < goombas.size(); i++)
	{
		Rectangle biggerBox = m_boundingBox;

		biggerBox._height += 2;

		if (biggerBox.checkCollision(goombas[i].getBoundingBox()))
		{
			collisionGoombas.push_back(goombas[i]);
		}
	}

	for (Goomba& goomba : collisionGoombas)
	{
		sides::Side side = m_boundingBox.getCollisionSide(goomba.getBoundingBox());

		switch (side)
		{
		case sides::TOP:
			break;

		case sides::BOTTOM:
			m_boxEmpty = true;
			break;

		case sides::LEFT:
			break;

		case sides::RIGHT:
			break;
		}
	}
}

void ItemBox::doAnimations()
{
	if (!m_boxEmpty)
	{
		AnimatedSprite::playAnimation("flashingBox");
	}
	else
	{
		AnimatedSprite::playAnimation("boxEmpty");
	}
}

void ItemBox::draw(Graphics& graphics, Rectangle& camera)
{
	SDL_Rect destRect = { (int)Sprite::m_x, (int)Sprite::m_y, Sprite::m_width, Sprite::m_height };

	//when drawing, need to offset the redBox by the camera distance
	destRect.x = destRect.x - (int)camera._x;
	destRect.y = destRect.y - (int)camera._y;

	AnimatedSprite::animateDraw(graphics, (int)destRect.x, (int)destRect.y, destRect.w, destRect.h);
}

void ItemBox::setupAnimation()
{
	AnimatedSprite::addAnimation(3, 0, 216, "flashingBox", 16, 16);
	AnimatedSprite::addAnimation(1, 0, 232, "boxEmpty", 16, 16);
}
