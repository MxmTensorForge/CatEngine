#include "FrameBuffer.h"

#include <SDL3/SDL.h>

FrameBuffer::FrameBuffer(int width, int height)
	: _width(width), _height(height), _pixels(width * height, 0), _zBuffer(width * height, 1.0f) {}

void FrameBuffer::clear(Color color) {
	std::fill(_pixels.begin(), _pixels.end(), color.argb());
	std::fill(_zBuffer.begin(), _zBuffer.end(), 1.0f);
}
void FrameBuffer::setPixel(int x, int y, Color color, float z) {
    x = clamp(x, 0, _width - 1);
    y = clamp(y, 0, _height - 1);
    size_t idx = x + y * _width;

    if (z < _zBuffer[idx]) {
        _pixels[idx] = color.argb();
        _zBuffer[idx] = z;
    }
}
void FrameBuffer::setPixelWithoutCheck(int x, int y, Color color, float z) {
    size_t idx = x + y * _width;

    if (z < _zBuffer[idx]) {
        _pixels[idx] = color.argb();
        _zBuffer[idx] = z;
    }
}
void FrameBuffer::setPixel(int x, int y, Color color) {
    x = clamp(x, 0, _width - 1);
    y = clamp(y, 0, _height - 1);
    size_t idx = x + y * _width;

    _pixels[idx] = color.argb();
}
void FrameBuffer::setPixelWithoutCheck(int x, int y, Color color) {
    size_t idx = x + y * _width;
    _pixels[idx] = color.argb();
}