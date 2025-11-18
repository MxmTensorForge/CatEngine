#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>
#include <cstdint>

#include "Color.h"

class FrameBuffer final
{
private:
	int _width, _height;
	std::vector<uint32_t> _pixels;
	std::vector<float> _zBuffer;

	int clamp(int val, int minVal, int maxVal) {
		return (val < minVal) ? minVal : (val > maxVal ? maxVal : val);
	}
public:
	FrameBuffer(int width, int height);

	void clear(Color color);
	void setPixel(int x, int y, Color color, float z);
	void setPixel(int x, int y, Color color);
	void setPixelWithoutCheck(int x, int y, Color color, float z);
	void setPixelWithoutCheck(int x, int y, Color color);

	inline int width() const noexcept { return _width; }
	inline int height() const noexcept { return _height; }

	inline const uint32_t* data() const noexcept { return _pixels.data(); }
};

#endif // !FRAMEBUFFER_H
