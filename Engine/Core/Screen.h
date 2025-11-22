#ifndef SCREEN_H
#define SCREEN_H

#include <SDL3/SDL.h>
#include "../Graphics/FrameBuffer.h"

class Screen final
{
private:
	SDL_Window* _window = nullptr;

	SDL_Renderer* _renderer = nullptr;
	SDL_Texture* _texture = nullptr;

	SDL_Event _event{};

public:
	Screen() = default;

	bool open(int width, int height);
	void close();
	bool pollEvents();
	void drawFBO(const FrameBuffer& fbo);
	void present();
	void clear();

	SDL_Renderer* getSDLRendererUnsafe() { return _renderer; }
};

#endif // !SCREEN_H