#include "Material.h"

Material::Material(Color color) : _color(color), _isShaded{ true } {}
Material::Material(const std::string& textureName) : _textureName(textureName), _isShaded{ true } {}