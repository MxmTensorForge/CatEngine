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

	if (!_screen.open(_width, _height)) return;
	auto& sceneManager = SceneManager::getInstance();

	_renderer.init();

	start();
	sceneManager.processPendingScene();

	float animAccumulator = 0.0f;
	float physAccumulator = 0.0f;

	bool isRunning = true;
	while (isRunning)
	{
		Input::update();
		isRunning = _screen.pollEvents();
		UISystem::getInstance().newFrame();

		Time::update();

		if (sceneManager.hasPendingScene()) {
			sceneManager.processPendingScene();
		}

		Time::begin("collisions");
		physAccumulator += Time::deltaTime();
		while (physAccumulator >= Time::fixedDeltaTime()) {
			sceneManager.getActiveScene()->updatePhysics();
			sceneManager.getActiveScene()->updateCollisions();
			fixedUpdate();

			physAccumulator -= Time::fixedDeltaTime();
		}
		Time::end("collisions");

		Time::begin("game update");
		update();
		sceneManager.getActiveScene()->update();
		Time::end("game update");

		Time::begin("animations");
		sceneManager.getActiveScene()->updateAnimator();
		Time::end("animations");

		auto camera = sceneManager.getActiveScene()->getMainCamera();
		if (camera) {
			_renderer.update(sceneManager.getActiveScene()->getMainCamera()->getComponent<Camera>());
		}
		_renderer.clear(Mxm::Vec4(_backgroundColor.rf(), _backgroundColor.gf(), _backgroundColor.bf(), _backgroundColor.af()));
		
		Time::begin("projection");

		for (const auto& obj : sceneManager.getActiveScene()->getGameObjects()) {
			if (!obj->getActive()) continue;

			auto mesh_ptr = obj->getComponent<MeshComponent>();
			if (!mesh_ptr) continue;

			auto& transform = obj->transform();

			_renderer.drawMesh(transform.getWorldMatrix(), mesh_ptr);
		}
		Time::end("projection");

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

		//_renderer.present(_screen);
		//UISystem::getInstance().render(_screen.getSDLRendererUnsafe());

		_screen.swap();
	}
	shutdown();
	_screen.close();
}

void Application::setDrawFrame(bool state) noexcept {
	_renderer.setDrawFrame(state);
}
void Application::setBackgroundColor(Color color) noexcept {
	_backgroundColor = color;
}

Application::Application() : _width(EngineConsts::STANDART_WIDTH), _height(EngineConsts::STANDART_HEIGHT), _screen() {}