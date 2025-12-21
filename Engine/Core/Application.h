#ifndef APPLICATION_H
#define APPLICATION_H

#include "Screen.h"
#include "../Graphics/Renderer.h"
#include "../Geometry/Triangle.h"

#include "../ComponentSystem/SceneManager.h"

class Application
{
private:
	Screen _screen;
	Renderer _renderer;

	int _width, _height;
	bool _isDrawingFrame = false;

	Color _backgroundColor = Color(255, 255, 255, 255);
protected:
	virtual void start() {}
	virtual void update() {}
	virtual void fixedUpdate() {}
	virtual void shutdown() {}

	void setDrawFrame(bool state) noexcept;
	void setBackgroundColor(Color color) noexcept;
public:
	Application();
	virtual ~Application() = default;
	
	void run();
};

#endif // !APPLICATION_H
