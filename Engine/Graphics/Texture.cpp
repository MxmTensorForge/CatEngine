#include "Texture.h"
#include "../Core/Logger.h"

#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/stb_image.h"

Texture::Texture() : _id(0), _width(0), _height(0), _channels(0) {}

Texture::Texture(const std::string& path) : Texture() {
	load(path);
}

Texture::~Texture() {
	if (_id) glDeleteTextures(1, &_id);
}

void Texture::load(const std::string& path) noexcept {
	stbi_set_flip_vertically_on_load(true);
	int width, height, channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

	if (!data) {
		Logger::getInstance().log(LogType::Fatal, "Image load failed");
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	_id = texture;
}
void Texture::bind(GLenum texture) const noexcept {
	glBindTexture(GL_TEXTURE_2D, _id);
	glActiveTexture(texture);
}