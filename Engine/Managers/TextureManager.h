#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <unordered_map>
#include <string>
#include <memory>

class Texture;

class TextureManager final
{
private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> _textures;

	TextureManager() = default;
	~TextureManager() = default;
public:
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager(TextureManager&&) = delete;
	TextureManager& operator=(TextureManager&&) = delete;

	static TextureManager& getInstance();

	void loadTextureFromFile(const std::string& name, const std::string& path);
	const std::unique_ptr<Texture>& getTexture(const std::string& name) const;

	void removeTexture(const std::string& name);
	void clearTextures(const std::string& name);
};

#endif // !RESOURCEMANAGER_H
