#ifndef APPLICATION_H
#define APPLICATION_H

#include "Screen.h"
#include "../Graphics/Render.h"
#include "../Geometry/Triangle.h"

#include "../ComponentSystem/SceneManager.h"

class Application
{
private:
	SceneManager _sceneManager;
	Screen _screen;
	Render _render;

	int _width, _height;
	bool _isDrawingFrame = false;

	Color _backgroundColor = Color(255, 255, 255, 255);

	std::vector<Triangle> _projectedTriangles;
protected:
	virtual void start() {}
	virtual void update() {}
	virtual void shutdown() {}

	void setDrawFrame(bool state) noexcept;
	void setBackgroundColor(Color color) noexcept;

	SceneManager& getSceneManager();
public:
	Application();
	virtual ~Application() = default;
	
	void run();
};

#endif // !APPLICATION_H
