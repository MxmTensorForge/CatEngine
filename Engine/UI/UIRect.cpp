#include "UIRect.h"
#include <SDL3/SDL.h>

UIRect::UIRect(const Mxm::Vec2i& pos, const Mxm::Vec2i& size, Color color) : UIElement(pos, size), _color(color) {

}

void UIRect::render(SDL_Renderer* renderer) const noexcept {
	SDL_SetRenderDrawColor(renderer, _color.r(), _color.g(), _color.b(), _color.a());
	SDL_FRect sdlRect = { _pos.x, _pos.y, _size.x, _size.y };
	SDL_RenderFillRect(renderer, &sdlRect);
}