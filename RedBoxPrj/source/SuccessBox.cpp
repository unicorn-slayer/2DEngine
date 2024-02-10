#include "SuccessBox.h"
#include "Goomba.h"
#include <algorithm>



SuccessBox::SuccessBox(float x, float y)
	: m_boundingBox(x, y, 32, 32)
	, m_playOnce(true)
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
			if (m_playOnce)
			{
				Mix_PauseMusic();
				MusicPlayer::getInstance().playSound("music/levelComplete.wav");


				m_playOnce = false;
			}
			return true;

		}
	}

	return false;
}


