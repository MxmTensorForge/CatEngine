#ifndef APPLICATION_H
#define APPLICATION_H

#include "Screen.h"
#include "../Graphics/Renderer.h"
#include "../UI/UIRenderer.h"

class MeshComponent;

class Application
{
private:
	Screen _screen;
	Renderer _renderer;
	UIRenderer _uiRenderer;

	int _width, _height;
	float _physAccumulator = 0.0f;
	float _stateAccumulator = 0.0f;

	Color _backgroundColor = Color(255, 255, 255, 255);
	std::vector<MeshComponent*> _transparentMeshes;
protected:
	virtual void start() {}
	virtual void update() {}
	virtual void fixedUpdate() {}
	virtual void shutdown() {}

	void setDrawFrame(bool state) noexcept;
	void setBackgroundColor(Color color) noexcept;

	void initialize();
	void updatePhysics();
	void updateGame();

	void renderOpaque();
	void renderTransparent();

	void renderFrame();
	void renderUI();

	void applySceneChanges();

	bool processFrame();
	void swapBuffers();
public:
	Application();
	virtual ~Application() = default;
	
	void run();
};

#endif // !APPLICATION_H
