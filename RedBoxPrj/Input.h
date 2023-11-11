#pragma once
#include <SDL.h>
#include <map>

class Input {
public:

	void keyDownEvent(const SDL_Event& event);
	void keyUpEvent(const SDL_Event& event);
	bool heldKeys(SDL_Scancode key);

private:
	std::map<SDL_Scancode, bool> _heldKeys;
};