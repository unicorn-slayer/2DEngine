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

// ISSUE: Ensure resources are deallocated


class MusicPlayer {
public:
    static MusicPlayer& getInstance() {
        static MusicPlayer instance;
        return instance;
    }

    void playMusic(const char* musicFile) {
        // Load and play the music
        Mix_Music* music = Mix_LoadMUS(musicFile);
        if (music != nullptr) {
            Mix_PlayMusic(music, -1); // -1 for infinite loop
        }
    }

    void playSound(const char* soundFile) {
        // Load and play the music
        Mix_Chunk* sound = Mix_LoadWAV(soundFile);
        if (sound != nullptr) {
            Mix_PlayChannel(-1, sound, 0); // -1 for infinite loop
        }
    }


private:
    MusicPlayer() {
        // Initialize SDL_mixer here if needed
    }

    // Disable copy constructor and assignment operator
    MusicPlayer(const MusicPlayer&) = delete;
    MusicPlayer& operator=(const MusicPlayer&) = delete;
};
