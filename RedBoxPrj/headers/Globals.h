#pragma once
#include <SDL_mixer.h>


namespace globals {

	const int g_screenWidth = 700;
	const int g_screenHeight = 700;
	const int g_mapWidth = 1920;
	const int g_mapHeight = 700;
	const int g_tileWidth = 32;
	const int g_tileHeight = 32;

	const float g_centreX = ((g_screenWidth - g_tileWidth) / 2);
	const float g_centreY = ((g_screenHeight - g_tileHeight) / 2);

	const float g_gravityCap = 20.0f; //0.8f
}

namespace sides {
	enum Side {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};

}

