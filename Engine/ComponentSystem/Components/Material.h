#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Component.h"
#include "../../Graphics/Color.h"

#include <string>

class Material final : public Component
{
private:
	std::string _textureName;
	Color _color;

	bool _isShaded;
public:
	Material(Color color);
	Material(const std::string& textureName);

	inline void setTextureName(const std::string& name) noexcept { _textureName = name; }
	inline const std::string& getTextureName() const noexcept { return _textureName; }

	inline void setColor(Color color) noexcept { _color = color; }
	inline const Color& getColor() const noexcept { return _color; }

	inline void setShaded(bool state) noexcept { _isShaded = state; }
	inline bool isShaded() const noexcept { return _isShaded; }
};

#endif