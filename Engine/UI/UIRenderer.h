#ifndef UIRENDERER_H
#define UIRENDERER_H

#include "../Graphics/VertexArray.h"
#include "../Graphics/Buffer.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"

#include <memory>
#include <vector>
#include <unordered_map>

#include "../Graphics/Color.h"
#include "../Mxm/Mat4.h"

struct RectCmd
{
	float x, y, z;
	float w, h;
	Color color;
};

struct TextCmd
{
	float x, y, z;
	float scale;
	std::string text;
	Color color;
};

struct CharData
{
	int x = 0, y = 0;
	int width = 0, height = 0;
	int xoffset = 0, yoffset = 0;
	int xadvance = 0;
};

class UIRenderer final
{
private:
	std::unique_ptr<VertexArray> _rectVAO;
	std::unique_ptr<Buffer> _rectVBO;
	std::unique_ptr<Shader> _rectShader;

	std::vector<float> _rectData;
	unsigned int _numVerts;

	std::unique_ptr<VertexArray> _textVAO;
	std::unique_ptr<Buffer> _textVBO;
	std::unique_ptr<Shader> _textShader;

	std::vector<float> _textData;
	unsigned int _numTextVerts;

	std::unique_ptr<Texture> _textTexture;
	std::unordered_map<int, CharData> _fontData;
	int _lineHeight;

	void addVertex(float x, float y, float z, Color color);
	void addTextVertex(float x, float y, float z, float u, float v, Color color);

	Mxm::Mat4 _ortho;
public:
	UIRenderer();
	~UIRenderer();

	void init(int width, int height);
	void loadFont(const std::string& fntPath, const std::string& texturePath);

	void pushRect(const RectCmd& r);
	void pushText(const TextCmd& r);
	void flush();
};

#endif // !UIRENDERER_H
