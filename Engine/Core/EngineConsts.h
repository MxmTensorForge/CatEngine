#ifndef ENGINECONSTS_H
#define ENGINECONSTS_H

namespace EngineConsts
{
	constexpr int STANDART_WIDTH = 1920;
	constexpr int STANDART_HEIGHT = 1080;

	constexpr const char* VERTEX_SHADER_PATH = "shaders/shader.vert";
	constexpr const char* FRAGMENT_SHADER_PATH = "shaders/shader.frag";

	constexpr const char* VERTEX_RECT_SHADER_PATH = "shaders/shaderUI.vert";
	constexpr const char* FRAGMENT_RECT_SHADER_PATH = "shaders/shaderUI.frag";

	constexpr const char* VERTEX_TEXT_SHADER_PATH = "shaders/shaderTextUI.vert";
	constexpr const char* FRAGMENT_TEXT_SHADER_PATH = "shaders/shaderTextUI.frag";

	constexpr const char* FONT_FNT_FILE_PATH = "font/font.fnt";
	constexpr const char* FONT_IMAGE_FILE_PATH = "font/font.png";
}

#endif // !ENGINECONSTS_H
