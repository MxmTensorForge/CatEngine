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

	float _shininess = 64.0f;
	float _specular = 0.5f;

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

	inline void setShininess(float value) noexcept { _shininess = value; }
	inline void setSpecular(float value) noexcept { _specular = value; }

	inline float getShininess() const noexcept { return _shininess; }
	inline float getSpecular() const noexcept { return _specular; }
};

#endif