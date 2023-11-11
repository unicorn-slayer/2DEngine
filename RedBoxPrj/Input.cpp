#include "Input.h"

void Input::keyDownEvent(const SDL_Event& event)
{
	this->_heldKeys[event.key.keysym.scancode] = true;
}

void Input::keyUpEvent(const SDL_Event& event)
{
	this->_heldKeys[event.key.keysym.scancode] = false;
}

bool Input::heldKeys(SDL_Scancode key)
{
	return this->_heldKeys[key];
}
