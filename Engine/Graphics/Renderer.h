#ifndef RENDER_H
#define RENDER_H

#include "FrameBuffer.h"
#include "Color.h"

class Screen;

class Renderer
{
private:
    FrameBuffer _fbo;

public:
    Renderer(int width, int height);

    void clear(Color color);
    void present(Screen& screen) const;

    void drawRect(int x, int y, int width, int height, Color color);
    void drawLine(int x0, int y0, float z0, int x1, int y1, float z1, Color color);
    void drawTriangleFrame(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, Color color);
    void drawTriangle(int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, Color color);
};

#endif // RENDER_H
