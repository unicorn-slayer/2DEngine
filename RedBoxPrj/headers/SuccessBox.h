#pragma once
#include <iostream>
#include "Sprite.h"
#include "Globals.h"


class Goomba;

class SuccessBox
{
public:
	SuccessBox(float x, float y);

	bool update(std::vector<Goomba>& goombas);

	Rectangle	m_boundingBox;


private:
	
	bool m_playOnce;

};