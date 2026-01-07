#include "SceneManager.h"
#include "../Core/Logger.h"

Scene* SceneManager::createScene() {
	_scenes.push_back(std::make_unique<Scene>());
	return _scenes.back().get();
}

void SceneManager::setActiveScene(Scene* scene) {
	if (scene) _pendingActiveScene = scene;
}

Scene* SceneManager::getActiveScene() const {
	if (_indexActiveScene < _scenes.size()) {
		return _scenes[_indexActiveScene].get();
	}
	Logger::getInstance().log(LogType::Fatal, "getActiveScene error GameManager");
	return nullptr;
}

void SceneManager::processPendingScene() {
	if (!_pendingActiveScene) return;

	for (size_t i = 0; i < _scenes.size(); i++) {
		Scene* scene = _scenes[i].get();
		if (scene == _pendingActiveScene) {
			scene->start();
			_indexActiveScene = i;
			Logger::getInstance().log(LogType::Message, "Scene index activated: " + std::to_string(i));
			break;
		}
	}

	_pendingActiveScene = nullptr;
}
bool SceneManager::hasPendingScene() {
	return _pendingActiveScene;
}

void SceneManager::removeScene(Scene* scene) {
	auto it = std::find_if(_scenes.begin(), _scenes.end(), [scene](const std::unique_ptr<Scene>& uniq) { return uniq.get() == scene; });
	if (it != _scenes.end()) {
		if (_scenes[_indexActiveScene].get() == scene) {
			_indexActiveScene = 0;
			_pendingActiveScene = nullptr;
		}
		_scenes.erase(it);
	}
}