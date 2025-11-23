#include "UIText.h"
#include "FontData.h"
#include <SDL3/SDL.h>

UIText::UIText(const Mxm::Vec2i& pos, const Mxm::Vec2i& size, const std::string& text, int scale, Color color) : UIElement(pos, size), _color(color), _text(text), _scale(scale) {

}

void UIText::render(SDL_Renderer* renderer) const noexcept {
	if (_text.empty()) return;

	Mxm::Vec2i currentPos = _pos;
	int lineHeight = FontData::myFontHeight * _scale;
	int charWidth = FontData::myFontWidth * _scale + 2;
	SDL_SetRenderDrawColor(renderer, _color.r(), _color.g(), _color.b(), _color.a());

	for (char c : _text) {
		if (currentPos.x + charWidth > _pos.x + _size.x) {
			currentPos.x = _pos.x;
			currentPos.y += lineHeight + FontData::myFontIndentY;

			if (currentPos.y + lineHeight > _pos.y + _size.y) {
				break;
			}
		}

		auto it = FontData::myFont.find(c);
		if (it == FontData::myFont.end()) continue;

		const FontData::FontChar& glyph = it->second;
		for (int row = 0; row < FontData::myFontHeight; row++) {
			for (int col = 0; col < FontData::myFontWidth; col++) {
				if (glyph.rows[row][col] != ' ') {
					SDL_FRect pixel = {
						static_cast<float>(currentPos.x + col * _scale),
						static_cast<float>(currentPos.y + row * _scale),
						static_cast<float>(_scale),
						static_cast<float>(_scale)
					};
					SDL_RenderFillRect(renderer, &pixel);
				}
			}
		}

		currentPos.x += FontData::myFontWidth * _scale + FontData::myFontIndentX;
	}
}