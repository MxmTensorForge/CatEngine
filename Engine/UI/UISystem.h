#ifndef UISYSTEM_H
#define UISYSTEM_H

#include "../Mxm/Vec2.h"
#include "../Mxm/Vec2i.h"
#include "../Graphics/Color.h"

#include <string>
#include <vector>
#include <memory>

class UICommand {
public:
	virtual ~UICommand() = default;
};
struct UIRect : public UICommand {
	Mxm::Vec2i pos;
	Mxm::Vec2i size;
	Color color;
};
struct UIText : public UICommand {
	Mxm::Vec2i pos;
	int scale = 1;
	std::string text;
	Color color;
};

struct SDL_Renderer;

class UISystem final
{
private:
	struct MouseData
	{
		Mxm::Vec2 _mousePos{};
		bool _mouseDown = false;
	} _mouseData;

	std::vector<std::unique_ptr<UICommand>> _renderCommands;

	UISystem() = default;
	~UISystem() = default;
public:
	UISystem(const UISystem&) = delete;
	UISystem& operator=(const UISystem&) = delete;
	UISystem(UISystem&&) = delete;
	UISystem& operator=(UISystem&&) = delete;

	static UISystem& getInstance() {
		static UISystem sys;
		return sys;
	}

	void drawQuad(const Mxm::Vec2i& pos, const Mxm::Vec2i& size, Color color) noexcept;
	void drawText(const Mxm::Vec2i& pos, const std::string& text, int scale, Color color) noexcept;
	bool drawButton(const Mxm::Vec2i& pos, const Mxm::Vec2i& size, const std::string& text, int scale, Color color) noexcept;

	void renderCommands(SDL_Renderer* renderer) noexcept;
};

#endif // !UISYSTEM_H
