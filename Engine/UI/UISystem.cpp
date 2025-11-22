#include "UISystem.h"
#include "TextData.h"

#include <SDL3/SDL.h>
#include <iostream>

void UISystem::drawQuad(const Mxm::Vec2i& pos, const Mxm::Vec2i& size, Color color) noexcept {
	auto rect = std::make_unique<UIRect>();
	rect->pos = pos;
	rect->size = size;
	rect->color = color;
	_renderCommands.push_back(std::move(rect));
}
void UISystem::drawText(const Mxm::Vec2i& pos, const std::string& text, int scale, Color color) noexcept {
	auto txt = std::make_unique<UIText>();
	txt->pos = pos;
	txt->text = text;
	txt->scale = scale;
	txt->color = color;
	_renderCommands.push_back(std::move(txt));
}
bool UISystem::drawButton(const Mxm::Vec2i& pos, const Mxm::Vec2i& size, const std::string& text, int scale, Color color) noexcept {

}

void UISystem::renderCommands(SDL_Renderer* renderer) noexcept {
	if (!renderer) return;

	for (const auto& cmd : _renderCommands) {
		if (auto* rect = dynamic_cast<UIRect*>(cmd.get())) {
			SDL_SetRenderDrawColor(renderer, rect->color.r(), rect->color.b(), rect->color.g(), rect->color.a());
			SDL_FRect sdlRect = { rect->pos.x, rect->pos.y, rect->size.x, rect->size.y };
			SDL_RenderFillRect(renderer, &sdlRect);
		}
		else if (auto* txt = dynamic_cast<UIText*>(cmd.get())) {
			Mxm::Vec2i currentPos = txt->pos;
			for (char c : txt->text) {
				auto it = FontData::myFont.find(c);
				if (it == FontData::myFont.end()) {
					continue;
				}

				const FontData::FontChar& g = it->second;
				for (int row = 0; row < FontData::myFontHeight; row++) {
					for (int col = 0; col < FontData::myFontWidth; col++) {
						if (g.rows[row][col] != ' ') {
							SDL_SetRenderDrawColor(renderer, txt->color.r(), txt->color.b(), txt->color.g(), txt->color.a());
							SDL_FRect sdlRect = { currentPos.x + col * txt->scale, currentPos.y + row * txt->scale, txt->scale, txt->scale };
							SDL_RenderFillRect(renderer, &sdlRect);
						}
					}
				}
				currentPos.x += FontData::myFontWidth * txt->scale + 3;
			}
		}
	}
	_renderCommands.clear();
}