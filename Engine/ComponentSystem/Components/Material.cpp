#include "Material.h"

Material::Material(Color color) : _color(color) {}
Material::Material(const std::string& textureName) : _textureName(textureName) {}