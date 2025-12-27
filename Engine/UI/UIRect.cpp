#include "UIRect.h"
#include "UIRenderer.h"

UIRect::UIRect(const Mxm::Vec2i& pos, const Mxm::Vec2i& size, Color color) : UIElement(pos, size), _color(color) {

}

void UIRect::render(UIRenderer& renderer) const noexcept {
	renderer.pushRect({ (float)_pos.x, (float)_pos.y, 0.0f, (float)_size.x, (float)_size.y, _color });
}