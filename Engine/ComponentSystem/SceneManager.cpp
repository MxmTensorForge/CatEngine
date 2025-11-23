#include "SceneManager.h"
#include "../Core/Logger.h"

std::shared_ptr<Scene> SceneManager::createScene() {
	auto scene = std::make_shared<Scene>();
	_scenes.emplace_back(scene);

	return scene;
}

void SceneManager::setActiveScene(const std::shared_ptr<Scene>& scene) {
	if (scene) _pendingActiveScene = scene;
}

std::shared_ptr<Scene> SceneManager::getActiveScene() const {
	if (_indexActiveScene < _scenes.size()) {
		return _scenes[_indexActiveScene];
	}
	Logger::getInstance().log(LogType::Fatal, "getActiveScene error GameManager");
	return nullptr;
}

void SceneManager::processPendingScene() {
	if (!_pendingActiveScene) return;

	for (size_t i = 0; i < _scenes.size(); i++) {
		if (_scenes[i] == _pendingActiveScene) {
			_scenes[i]->start();
			_indexActiveScene = i;
			Logger::getInstance().log(LogType::Message, "Scene index activated: " + std::to_string(i));
			break;
		}
	}

	_pendingActiveScene = nullptr;
}
bool SceneManager::hasPendingScene() {
	return _pendingActiveScene.get();
}

void SceneManager::removeScene(const std::shared_ptr<Scene>& scene) {
	auto it = std::find(_scenes.begin(), _scenes.end(), scene);
	if (it != _scenes.end()) {
		if (_scenes[_indexActiveScene] == scene) {
			_indexActiveScene = 0;
			_pendingActiveScene = nullptr;
		}
		_scenes.erase(it);
	}
}