#include "UIRenderer.h"

#include <iostream>
#include <fstream>
#include <sstream>

UIRenderer::UIRenderer() : _numVerts(0), _numTextVerts(0) {
	_rectData.reserve(200 * 7);
	_textData.reserve(100 * 9);
}
UIRenderer::~UIRenderer() {

}

void UIRenderer::init(int width, int height) {
	_rectVAO = std::make_unique<VertexArray>();
	_rectVBO = std::make_unique<Buffer>(GL_ARRAY_BUFFER);
	_rectShader = std::make_unique<Shader>("shaders/shaderUI.vert", "shaders/shaderUI.frag");

	_rectVAO->bind();
	_rectVBO->bind();

	_rectVAO->setAttribute(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	_rectVAO->setAttribute(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

	_rectVAO->enableAttribute(0);
	_rectVAO->enableAttribute(1);


	_textVAO = std::make_unique<VertexArray>();
	_textVBO = std::make_unique<Buffer>(GL_ARRAY_BUFFER);
	_textShader = std::make_unique<Shader>("shaders/shaderTextUI.vert", "shaders/shaderTextUI.frag");

	_textVAO->bind();
	_textVBO->bind();

	_textVAO->setAttribute(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	_textVAO->setAttribute(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	_textVAO->setAttribute(2, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(5 * sizeof(float)));

	_textVAO->enableAttribute(0);
	_textVAO->enableAttribute(1);
	_textVAO->enableAttribute(2);

	_ortho = Mxm::Mat4::ortho((float)width, 0.0f, 0.0f, (float)height, -1.0f, 1.0f);

	loadFont("font/myfont.fnt", "font/myfont_0.png");
}
void UIRenderer::loadFont(const std::string& fntPath, const std::string& texturePath) {
	_textTexture = std::make_unique<Texture>(texturePath, true);

	std::ifstream texFile(fntPath);
	if (!texFile.is_open()) {
		return;
	}

	std::string line;
	while (std::getline(texFile, line)) {
		if (line.find("char id=") != std::string::npos) {
			CharData data;
			int id = 0;

			std::istringstream ss(line);
			std::string token;

			while (ss >> token) {
				if (token.find("id=") != std::string::npos) id = std::stoi(token.substr(3));
				else if (token.find("x=") != std::string::npos) data.x = std::stoi(token.substr(2));
				else if (token.find("y=") != std::string::npos) data.y = std::stoi(token.substr(2));
				else if (token.find("width=") != std::string::npos) data.width = std::stoi(token.substr(6));
				else if (token.find("height=") != std::string::npos) data.height = std::stoi(token.substr(7));
				else if (token.find("xoffset=") != std::string::npos) data.xoffset = std::stoi(token.substr(8));
				else if (token.find("yoffset=") != std::string::npos) data.yoffset = std::stoi(token.substr(8));
				else if (token.find("xadvance=") != std::string::npos) data.xadvance = std::stoi(token.substr(9));
			}

			_fontData[id] = data;
		}
		else if (line.find("common") != std::string::npos) {
			size_t pos = line.find("lineHeight=");
			if (pos != std::string::npos) {
				size_t end = line.find(" ", pos);
				_lineHeight = std::stoi(line.substr(pos + 11, end - pos - 11));
			}
		}
	}

	texFile.close();
}

void UIRenderer::addVertex(float x, float y, float z, Color color) {
    _rectData.push_back(x);
    _rectData.push_back(y);
    _rectData.push_back(z);

    _rectData.push_back(color.rf());
    _rectData.push_back(color.gf());
    _rectData.push_back(color.bf());
    _rectData.push_back(color.af());

    _numVerts++;
}
void UIRenderer::addTextVertex(float x, float y, float z, float u, float v, Color color) {
	_textData.push_back(x);
	_textData.push_back(y);
	_textData.push_back(z);
	_textData.push_back(u);
	_textData.push_back(v);

	_textData.push_back(color.rf());
	_textData.push_back(color.gf());
	_textData.push_back(color.bf());
	_textData.push_back(color.af());

	_numTextVerts++;
}

void UIRenderer::pushRect(const RectCmd& r) {
    addVertex(r.x, r.y, r.z, r.color);
    addVertex(r.x + r.w, r.y, r.z, r.color);
    addVertex(r.x, r.y + r.h, r.z, r.color);

    addVertex(r.x + r.w, r.y, r.z, r.color);
	addVertex(r.x + r.w, r.y + r.h, r.z, r.color);
    addVertex(r.x, r.y + r.h, r.z, r.color);
}
void UIRenderer::pushText(const TextCmd& t) {
	if (_fontData.empty()) {
		return;
	}

	int cursorX = t.x;
	int cursorY = t.y;

	for (char c : t.text) {
		if (c == '\n') {
			cursorY += _lineHeight * t.scale;
			cursorX = t.x;
			continue;
		}

		auto it = _fontData.find(static_cast<int>(c));
		if (it == _fontData.end()) {
			continue;
		}

		const CharData& data = it->second;

		float xpos = (float)cursorX + (float)data.xoffset * t.scale;
		float ypos = (float)cursorY + (float)data.yoffset * t.scale;
		float w = (float)data.width * t.scale;
		float h = (float)data.height * t.scale;

		float texWidth = static_cast<float>(_textTexture->getWidth());
		float texHeight = static_cast<float>(_textTexture->getHeight());

		float u1 = (data.x) / texWidth;
		float v1 = (data.y) / texHeight;
		float u2 = (data.x + data.width) / texWidth;
		float v2 = (data.y + data.height) / texHeight;

		addTextVertex(xpos, ypos, t.z, u1, v1, t.color);
		addTextVertex(xpos + w, ypos, t.z, u2, v1, t.color);
		addTextVertex(xpos, ypos + h, t.z, u1, v2, t.color);

		addTextVertex(xpos + w, ypos, t.z, u2, v1, t.color);
		addTextVertex(xpos + w, ypos + h, t.z, u2, v2, t.color);
		addTextVertex(xpos, ypos + h, t.z, u1, v2, t.color);

		cursorX += data.xadvance * t.scale;
	}
}

void UIRenderer::flush() {
	if (_numVerts != 0) {
		_rectShader->use();
		_rectVAO->bind();

		_rectVBO->bind();
		_rectVBO->bufferData(_rectData.size() * sizeof(float), _rectData.data(), GL_STREAM_DRAW);

		_rectShader->setUniform("uProjection", _ortho.data(), true);
		glDrawArrays(GL_TRIANGLES, 0, _numVerts);

		_rectData.clear();
		_numVerts = 0;
	}
	if (_numTextVerts != 0) {
		_textShader->use();
		_textVAO->bind();

		_textVBO->bind();
		_textVBO->bufferData(_textData.size() * sizeof(float), _textData.data(), GL_STREAM_DRAW);

		_textTexture->bind(0);
		_textShader->setUniform("textTexture", 0);

		_textShader->setUniform("uProjection", _ortho.data(), true);
		glDrawArrays(GL_TRIANGLES, 0, _numTextVerts);

		_textData.clear();
		_numTextVerts = 0;
	}
}