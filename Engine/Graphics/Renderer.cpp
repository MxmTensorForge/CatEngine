#include "Renderer.h"
#include "../Core/Screen.h"
#include "../Mxm/Consts.h"

#include <cstdlib>

Renderer::Renderer(int width, int height) : _fbo(width, height) {}

void Renderer::clear(Color color) {
	_fbo.clear(color);
}
void Renderer::present(Screen& screen) const {
	screen.drawFBO(_fbo);
}

void Renderer::drawRect(int x, int y, int width, int height, Color color) {
    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + height; j++) {
            _fbo.setPixel(i, j, color);
        }
    }
}
void Renderer::drawLine(int x0, int y0, float z0, int x1, int y1, float z1, Color color) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    int steps = std::max(std::abs(dx), std::abs(dy));
    if (steps == 0) {
        return;
    }

    float xInc = (float)dx / steps;
    float yInc = (float)dy / steps;
    float zInc = (float)(z1-z0) / steps;

    float x = (float)x0;
    float y = (float)y0;
    float z = (float)z0;

    for (int i = 0; i < steps; i++) {
        _fbo.setPixel(std::round(x), std::round(y), color, z);

        x += xInc;
        y += yInc;
        z += zInc;
    }
}
void Renderer::drawTriangleFrame(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, Color color) {
	drawLine(x0, y0, z0, x1, y1, z1, color);
	drawLine(x1, y1, z1, x2, y2, z2, color);
	drawLine(x2, y2, z2, x0, y0, z0, color);
}
void Renderer::drawTriangle(
    int x0, int y0, float z0,
    int x1, int y1, float z1,
    int x2, int y2, float z2,
    Color color)
{
    int minX = std::max(0, std::min(std::min(x0, x1), x2));
    int maxX = std::min(_fbo.width() - 1, std::max(std::max(x0, x1), x2));
    int minY = std::max(0, std::min(std::min(y0, y1), y2));
    int maxY = std::min(_fbo.height() - 1, std::max(std::max(y0, y1), y2));

    int x1_x2 = x1 - x2;
    int x0_x2 = x0 - x2;
    int x2_x1 = x2 - x1;
    int x2_x0 = x2 - x0;

    int y1_y2 = y1 - y2;
    int y0_y2 = y0 - y2;
    int y2_y1 = y2 - y1;
    int y2_y0 = y2 - y0;

    float denom = (float)(y1_y2 * x0_x2 + x2_x1 * y0_y2);
    if (fabsf(denom) < Mxm::Consts::EPS) return;

    float invDenom = 1.0f / denom;

    float inc0X = (float)y1_y2 * invDenom;
    float inc0Y = (float)x2_x1 * invDenom;

    float inc1X = (float)y2_y0 * invDenom;
    float inc1Y = (float)x0_x2 * invDenom;

    float l0 = (float)(y1_y2 * (minX - x2) + x2_x1 * (minY - y2)) * invDenom;
    float l1 = (float)(y2_y0 * (minX - x2) + x0_x2 * (minY - y2)) * invDenom;

    for (int y = minY; y <= maxY; y++) {
        float l0_row = l0;
        float l1_row = l1;
        for (int x = minX; x <= maxX; x++) {
            float l2 = 1.0f - l0_row - l1_row;

            if (l0_row >= -0.001f && l1_row >= -0.001f && l2 >= -0.001f) {
                float z = l0_row * z0 + l1_row * z1 + l2 * z2;
                _fbo.setPixelWithoutCheck(x, y, color, z);
            }

            l0_row += inc0X;
            l1_row += inc1X;
        }
        l0 += inc0Y;
        l1 += inc1Y;
    }
}