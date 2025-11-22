#include "Screen.h"
#include "Logger.h"
#include "../Core/Input.h"
#include "../Core/EngineConsts.h"

bool Screen::open(int width, int height) {
	if (!SDL_Init(SDL_INIT_VIDEO)) 
		return false;

	_window = SDL_CreateWindow("main", width, height, 0);
	if (!_window) {
		Logger::getInstance().log(LogType::Fatal, "Window creation error [sdl error]");
		return false;
	}
	Logger::getInstance().log(LogType::Message, "Window created successfully");

	_renderer = SDL_CreateRenderer(_window, nullptr);
	if (!_renderer) {
		Logger::getInstance().log(LogType::Fatal, "Renderer creation error [sdl error]");
		return false;
	}
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
	Logger::getInstance().log(LogType::Message, "Renderer created successfully");

	_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width * EngineConsts::SCREEN_SCALE, height * EngineConsts::SCREEN_SCALE);
	if (!_texture) {
		Logger::getInstance().log(LogType::Fatal, "Texture creation error [sdl error]");
		return false;
	}
	SDL_SetTextureScaleMode(_texture, SDL_SCALEMODE_NEAREST);
	Logger::getInstance().log(LogType::Message, "Texture created successfully");

	if (!SDL_SetRenderVSync(_renderer, 1)) {
		Logger::getInstance().log(LogType::Error, "VSync enable error [sdl error]");
		return false;
	}
	Input::setWindow(_window);

	return true;
}
void Screen::close() {
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);

	SDL_Quit();
}
bool Screen::pollEvents() {
	while (SDL_PollEvent(&_event)) {
		if (_event.type == SDL_EVENT_QUIT) return false;
		Input::handleEvent(_event);
	}
	return true;
}

void Screen::drawFBO(const FrameBuffer& fbo) {
	SDL_UpdateTexture(_texture, nullptr, (void*)fbo.data(), fbo.width() * sizeof(uint32_t));

	SDL_Vertex vertices[4];

	int windowWidth, windowHeight;
	SDL_GetWindowSize(_window, &windowWidth, &windowHeight);

	vertices[0].position = { 0.0f, 0.0f };
	vertices[0].color = { 1, 1, 1, 1 };
	vertices[0].tex_coord = { 0.0f, 0.0f };

	vertices[1].position = { (float)windowWidth, 0.0f };
	vertices[1].color = { 1, 1, 1, 1 };
	vertices[1].tex_coord = { 1.0f, 0.0f };

	vertices[2].position = { (float)windowWidth, (float)windowHeight };
	vertices[2].color = { 1, 1, 1, 1 };
	vertices[2].tex_coord = { 1.0f, 1.0f };

	vertices[3].position = { 0.0f, (float)windowHeight };
	vertices[3].color = { 1, 1, 1, 1 };
	vertices[3].tex_coord = { 0.0f, 1.0f };

	int indices[6] = { 0, 1, 2, 2, 3, 0 };

	SDL_RenderGeometry(_renderer, _texture, vertices, 4, indices, 6);
}
void Screen::present() {
	SDL_RenderPresent(_renderer);
}
void Screen::clear() {
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);
}