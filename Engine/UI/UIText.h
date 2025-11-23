#ifndef UITEXT_H
#define UITEXT_H

#include "UIElement.h"
#include "../Graphics/Color.h"

#include <string>

class UIText final : public UIElement
{
private:
	std::string _text;
	int _scale = 1;
	Color _color;
public:
	UIText(const Mxm::Vec2i& pos, const Mxm::Vec2i& size, const std::string& text, int scale, Color color);
	void render(SDL_Renderer* renderer) const noexcept override;

	void setColor(Color color) noexcept { _color = color; }
	Color getColor() const noexcept { return _color; }

	void setText(const std::string& text) noexcept { _text = text; }
	const std::string& getText() const noexcept { return _text; }

	void setScale(int scale) noexcept { _scale = scale; }
	int getScale() const noexcept { return _scale; }
};

#endif // !UITEXT_H
