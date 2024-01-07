#pragma once
#include <iostream>
#include "AnimatedSprite.h"
#include "Globals.h"

class Graphics;

class Goomba;

class ItemBox : public AnimatedSprite
{
public:
	ItemBox(Graphics& graphics, float x, float y);

	void update(std::vector<Goomba>& goombas);

	void doAnimations();

	void draw(Graphics& graphics, Rectangle& camera);

	void setupAnimation();



private:

	bool m_boxEmpty;
};