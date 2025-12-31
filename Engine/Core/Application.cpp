#include "Application.h"

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
	_renderer.viewport(_width, _height);
	_uiRenderer.init(_width, _height);

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
		auto activeScene = sceneManager.getActiveScene();

		Time::begin("collisions");
		physAccumulator += Time::deltaTime();
		while (physAccumulator >= Time::fixedDeltaTime()) {
			activeScene->updatePhysics();
			activeScene->updateCollisions();
			fixedUpdate();

			physAccumulator -= Time::fixedDeltaTime();
		}
		Time::end("collisions");

		Time::begin("game update");
		update();
		activeScene->update();
		Time::end("game update");

		Time::begin("animations");
		activeScene->updateAnimator();
		Time::end("animations");

		auto camera = activeScene->getMainCamera();
		if (camera) {
			_renderer.update(activeScene->getMainCamera()->getComponent<Camera>(), activeScene->getPointLights(), activeScene->getDirectionLight());
		}
		_renderer.clear(Mxm::Vec4(_backgroundColor.rf(), _backgroundColor.gf(), _backgroundColor.bf(), _backgroundColor.af()));
		
		Time::begin("projection");

		_transparentMeshes.clear();
		for (const auto& obj : activeScene->getGameObjects()) {
			if (!obj->getActive()) continue;

			auto mesh_ptr = obj->getComponent<MeshComponent>();
			if (!mesh_ptr) continue;

			if (mesh_ptr->getColor().a() < 255) {
				_transparentMeshes.push_back(mesh_ptr);
				continue;
			}

			auto& transform = obj->transform();

			_renderer.drawMesh(transform.getWorldMatrix(), mesh_ptr);
		}

		for (const auto& mesh : _transparentMeshes) {
			auto obj = mesh->getObject();
			if (!obj->getActive()) continue;

			auto& transform = obj->transform();
			_renderer.drawMesh(transform.getWorldMatrix(), mesh);
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

		UISystem::getInstance().render(_uiRenderer);
		_uiRenderer.flush();

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

Application::Application() : _width(EngineConsts::STANDART_WIDTH), _height(EngineConsts::STANDART_HEIGHT), _screen(), _uiRenderer() {}