#include "Application.h"

#include "../ComponentSystem/Object/Transform.h"
#include "../ComponentSystem/Components/Camera.h"
#include "../ComponentSystem/Components/Material.h"
#include "../Animation/Animator.h"

#include "../ComponentSystem/SceneManager.h"

#include "../UI/UISystem.h"

#include "Logger.h"
#include "Time.h"
#include "EngineConsts.h"

#include "Input.h"

void Application::initialize() {
	_screen = std::make_unique<Screen>();
	_renderer = std::make_unique<Renderer>();
	_uiRenderer = std::make_unique<UIRenderer>();

	Logger::getInstance().setLogFile("log.txt");

	if (!_screen->open(_width, _height)) return;
	auto& sceneManager = SceneManager::getInstance();

	_renderer->init();
	_renderer->viewport(_width, _height);
	_uiRenderer->init(_width, _height);

	start();
	sceneManager.processPendingScene();

	_physAccumulator = 0.0f;
}
void Application::updatePhysics() {
	Time::begin("updatePhysics");

	auto& sceneManager = SceneManager::getInstance();
	auto activeScene = sceneManager.getActiveScene();

	_physAccumulator += Time::deltaTime();
	while (_physAccumulator >= Time::fixedDeltaTime()) {
		activeScene->updatePhysics();
		activeScene->updateCollisions();
		fixedUpdate();

		_physAccumulator -= Time::fixedDeltaTime();
	}

	Time::end("updatePhysics");
}
void Application::updateGame() {
	Time::begin("updateGame");

	auto& sceneManager = SceneManager::getInstance();
	auto activeScene = sceneManager.getActiveScene();

	update();
	activeScene->updateAnimator();
	activeScene->update();

	Time::end("updateGame");
}

void Application::renderOpaque() {
	auto& sceneManager = SceneManager::getInstance();
	auto activeScene = sceneManager.getActiveScene();

	_transparentMaterials.clear();
	for (const auto& obj : activeScene->getGameObjects()) {
		if (!obj->getActive()) continue;

		auto material_ptr = obj->getComponent<Material>();
		if (!material_ptr) continue;

		if (material_ptr->getColor().a() < 255) {
			_transparentMaterials.push_back(material_ptr);
			continue;
		}

		auto& transform = obj->transform();

		_renderer->drawMesh(transform.getWorldMatrix(), material_ptr);
	}
}
void Application::renderTransparent() {
	for (const auto& material : _transparentMaterials) {
		auto obj = material->getObject();
		if (!obj->getActive()) continue;

		auto& transform = obj->transform();
		_renderer->drawMesh(transform.getWorldMatrix(), material);
	}
}
void Application::renderFrame() {
	Time::begin("renderFrame");

	auto& sceneManager = SceneManager::getInstance();
	auto activeScene = sceneManager.getActiveScene();

	auto camera = activeScene->getMainCamera();
	if (camera) {
		_renderer->update(camera->getComponent<Camera>(), activeScene->getPointLights(), activeScene->getDirectionLight());
	}
	_renderer->setAmbientColor(activeScene->getAmbientColor());
	_renderer->clear(activeScene->getBackgroundColor());

	renderOpaque();
	renderTransparent();

	Time::end("renderFrame");
}
void Application::renderUI() {
	Time::begin("renderUI");

	UISystem::getInstance().render(_uiRenderer.get());
	_uiRenderer->flush();

	Time::end("renderUI");
}

void Application::applySceneChanges() {
	auto& sceneManager = SceneManager::getInstance();

	if (sceneManager.hasPendingScene()) {
		sceneManager.processPendingScene();
	}
}

bool Application::processFrame() {
	Input::update();
	if (!_screen->pollEvents()) return false;

	UISystem::getInstance().newFrame();

	Time::update();

	applySceneChanges();
	updatePhysics();
	updateGame();

	renderFrame();
	renderUI();

	swapBuffers();

	return true;
}
void Application::swapBuffers() {
	_screen->swap();
}

void Application::run() {
	initialize();

	bool isRunning = true;
	while (isRunning)
	{
		isRunning = processFrame();

		_stateAccumulator += Time::deltaTime();
		if (_stateAccumulator >= 1.0f) {
			_stateAccumulator = 0.0f;

			float update = Time::get("updateGame");
			float physics = Time::get("updatePhysics");
			float renderFrame = Time::get("renderFrame");
			float renderUI = Time::get("renderUI");

			Logger::getInstance().log(LogType::Message, "<- All: " + std::to_string(update + physics + renderFrame + renderUI) + " ms");
			Logger::getInstance().log(LogType::Message, "<  Game update: " + std::to_string(update) + " ms");
			Logger::getInstance().log(LogType::Message, "<  Physics update: " + std::to_string(physics) + " ms");
			Logger::getInstance().log(LogType::Message, "<  Rendering frame: " + std::to_string(renderFrame) + " ms");
			Logger::getInstance().log(LogType::Message, "<  Rendering ui: " + std::to_string(renderUI) + " ms");
		}
	}
	shutdown();
	_screen->close();
}

void Application::setDrawFrame(bool state) noexcept {
	_renderer->setDrawFrame(state);
}

Application::Application() : _width(EngineConsts::STANDART_WIDTH), _height(EngineConsts::STANDART_HEIGHT), _screen() {}