#include "Application.h"

#include "../ComponentSystem/Components/MeshComponent.h"
#include "../ComponentSystem/Object/Transform.h"
#include "../ComponentSystem/Components/Camera.h"
#include "../Animation/Animator.h"

#include "../UI/UISystem.h"

#include "Logger.h"
#include "Time.h"
#include "EngineConsts.h"

#include "Input.h"

#include <iostream>

void Application::run() {
	Logger::getInstance().setLogFile("log.txt");

	if (!_screen.open(_width / EngineConsts::SCREEN_SCALE, _height / EngineConsts::SCREEN_SCALE)) return;

	start();
	_sceneManager.getActiveScene()->start();

	float animAccumulator = 0.0f;
	float physAccumulator = 0.0f;

	bool isRunning = true;
	while (isRunning)
	{
		Input::update();
		isRunning = _screen.pollEvents();

		Time::update();

		Time::begin("collisions");
		physAccumulator += Time::deltaTime();
		while (physAccumulator >= Time::fixedDeltaTime()) {
			_sceneManager.getActiveScene()->updatePhysics();
			_sceneManager.getActiveScene()->updateCollisions();
			fixedUpdate();

			physAccumulator -= Time::fixedDeltaTime();
		}
		Time::end("collisions");

		Time::begin("game update");
		update();
		_sceneManager.getActiveScene()->update();
		Time::end("game update");

		Time::begin("animations");
		_sceneManager.getActiveScene()->updateAnimator();
		Time::end("animations");

		_screen.clear();
		_renderer.clear(_backgroundColor);
		
		Time::begin("projection");
		_projectedTriangles.clear();

		for (const auto& obj : _sceneManager.getActiveScene()->getGameObjects()) {
			if (!obj->getActive()) continue;

			auto mesh_ptr = obj->getComponent<MeshComponent>();
			if (!mesh_ptr) continue;

			auto& transform = obj->transform();

			auto& tris = _sceneManager.getActiveScene()->getMainCamera()->getComponent<Camera>()->project(mesh_ptr, transform);
			_projectedTriangles.insert(_projectedTriangles.end(), tris.begin(), tris.end());
		}
		Time::end("projection");

		Time::begin("rasterization");
		for (const auto& t : _projectedTriangles) {
			if (_isDrawingFrame) {
				_renderer.drawTriangleFrame(
					static_cast<int>(t[0].x), static_cast<int>(t[0].y), t[0].z,
					static_cast<int>(t[1].x), static_cast<int>(t[1].y), t[1].z,
					static_cast<int>(t[2].x), static_cast<int>(t[2].y), t[2].z,
					Color(255, 255, 255, 255));
			}
			else {
				_renderer.drawTriangle(
					static_cast<int>(t[0].x), static_cast<int>(t[0].y), t[0].z,
					static_cast<int>(t[1].x), static_cast<int>(t[1].y), t[1].z,
					static_cast<int>(t[2].x), static_cast<int>(t[2].y), t[2].z,
					t.color());
			}
		}
		Time::end("rasterization");

		animAccumulator += Time::deltaTime();
		if (animAccumulator >= 1.0f) {
			float proj = Time::get("projection");
			float rast = Time::get("rasterization");
			float upd = Time::get("game update");
			float anim = Time::get("animations");
			float collis = Time::get("collisions");

			float total = proj + rast + upd + anim + collis;

			std::cout << "\n\nProjection: " << proj << " ms\n"
				<< "Rasterization: " << rast << " ms\n"
				<< "Game update: " << upd << " ms\n"
				<< "Animations: " << anim << " ms\n"
				<< "Collisions: " << collis << " ms\n"
				<< "All: " << total << " ms\n";

			animAccumulator = 0.0f;
		}

		_renderer.drawRect(_width / 2 - 1, _height / 2 - 1, 2, 2, Color(255, 255, 255, 255));

		_renderer.present(_screen);
		UISystem::getInstance().renderCommands(_screen.getSDLRendererUnsafe());

		_screen.present();
	}
	shutdown();
	_screen.close();
}

void Application::setDrawFrame(bool state) noexcept {
	_isDrawingFrame = state;
}
void Application::setBackgroundColor(Color color) noexcept {
	_backgroundColor = color;
}

SceneManager& Application::getSceneManager() {
	return _sceneManager;
}

Application::Application() : _width(EngineConsts::SCALED_WIDTH), _height(EngineConsts::SCALED_HEIGHT), _renderer(EngineConsts::SCALED_WIDTH, EngineConsts::SCALED_HEIGHT), _screen() {}