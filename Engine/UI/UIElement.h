#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "../Mxm/Vec2i.h"

class UIRenderer;

class UIElement
{
protected:
	Mxm::Vec2i _pos;
	Mxm::Vec2i _size;
	bool _visible = false;
public:
	virtual ~UIElement() = default;
	virtual void render(UIRenderer& renderer) const noexcept = 0;
	virtual void update() noexcept {}

	UIElement(const Mxm::Vec2i& pos, const Mxm::Vec2i& size)
		: _pos(pos), _size(size) {
	}

	void setPosition(const Mxm::Vec2i& pos) noexcept { _pos = pos; }
	void addPosition(const Mxm::Vec2i& pos) noexcept { _pos += pos; }

	void setSize(const Mxm::Vec2i& size) noexcept { _size = size; }
	void addSize(const Mxm::Vec2i& size) noexcept { _size += size; }

	void setVisible(bool visible) noexcept { _visible = visible; }

	const Mxm::Vec2i& getPosition() const noexcept { return _pos; }
	const Mxm::Vec2i& getSize() const noexcept { return _size; }

	bool getVisible() const noexcept { return _visible; }
	bool isInside(const Mxm::Vec2i& point) const noexcept {
		return point.x >= _pos.x && point.x <= _pos.x + _size.x &&
			   point.y >= _pos.y && point.y <= _pos.y + _size.y;
	}
};

#endif // !UIELEMENT_H
