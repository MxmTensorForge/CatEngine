#include "TextureManager.h"
#include "../Graphics/Texture.h"

#include "Logger.h"
#include <utility>

TextureManager& TextureManager::getInstance() {
	static TextureManager manager;
	return manager;
}

void TextureManager::loadTexture(const std::string& name, const std::string& path) {
	std::unique_ptr<Texture> tex = std::make_unique<Texture>(path, false);
	_textures[name] = std::move(tex);

	Logger::getInstance().log(LogType::Message, "Texture loaded successfully: " + name);
}
const std::unique_ptr<Texture>& TextureManager::getTexture(const std::string& name) const {
	auto it = _textures.find(name);
	if (it == _textures.end()) {
		Logger::getInstance().log(LogType::Fatal, "Texture not found (" + name + ").");
		return nullptr;
	}

	return it->second;
}

void TextureManager::removeTexture(const std::string& name) {
	auto it = _textures.find(name);
	if (it == _textures.end()) return;

	_textures.erase(it);
}
void TextureManager::clearTextures(const std::string& name) {
	_textures.clear();
}