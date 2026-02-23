#include "UIText.h"
#include "UIRenderer.h"

UIText::UIText(const Mxm::Vec2i& pos, const std::string& text, float scale, Color color) : UIElement(pos, Mxm::Vec2i(0, 0)), _color(color), _text(text), _scale(scale) {

}

void UIText::render(UIRenderer* renderer) const noexcept {
	if (_text.empty()) return;

	renderer->pushText({(float)_pos.x, (float)_pos.y, -0.1f, _scale, _text, _color });
}