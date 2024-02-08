#include "SuccessBox.h"
#include "Goomba.h"
#include <algorithm>



SuccessBox::SuccessBox(float x, float y)
	: m_boundingBox(x, y, 32, 32)
{


}

bool SuccessBox::update(std::vector<Goomba>& goombas)
{
	std::vector<Goomba> collisionGoombas;

	// handle collisions
	for (int i = 0; i < goombas.size(); i++)
	{

		if (m_boundingBox.checkCollision(goombas[i].getBoundingBox()))
		{
			return true;
		}
	}

	return false;
}


