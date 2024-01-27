#include "Graphics.h"
#include <SDL_image.h>

Graphics::Graphics()
{
	SDL_CreateWindowAndRenderer(700, 700, 0, &this->_window, &this->_renderer);
	SDL_SetWindowTitle(this->_window, "RedBox");
}

Graphics::~Graphics()
{
	// Below still needs done just somewhere else

	//SDL_DestroyWindow(this->_window);
	//SDL_DestroyRenderer(this->_renderer);
}

SDL_Surface* Graphics::loadImage(const std::string& filePath)
{
	if (this->_spriteSheets.count(filePath) == 0) {
		this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());
		SDL_SetColorKey(this->_spriteSheets[filePath], SDL_TRUE, SDL_MapRGB(this->_spriteSheets[filePath]->format, 0x92, 0x90, 0xFF));
	}
	return this->_spriteSheets[filePath];
}

void Graphics::clear()
{
	SDL_RenderClear(this->_renderer);
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle)
{
	SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
}

void Graphics::flip()
{
	SDL_RenderPresent(this->_renderer);
}

SDL_Renderer* Graphics::getRenderer() const
{
	return this->_renderer;
}