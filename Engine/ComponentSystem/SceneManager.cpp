#include "SceneManager.h"
#include "../Core/Logger.h"

std::shared_ptr<Scene> SceneManager::createScene() {
	auto scene = std::make_shared<Scene>();
	_scenes.emplace_back(scene);

	return scene;
}
void SceneManager::setActiveScene(const std::shared_ptr<Scene>& scene) {
	for (size_t i = 0; i < _scenes.size(); i++)
	{
		if (_scenes[i] == scene) {
			_indexActiveScene = i;
			return;
		}
	}
	Logger::getInstance().log(LogType::Fatal, "scene not found in GameManager");
}

std::shared_ptr<Scene> SceneManager::getActiveScene() const {
	if (_indexActiveScene < _scenes.size()) {
		return _scenes[_indexActiveScene];
	}
	Logger::getInstance().log(LogType::Fatal, "getActiveScene error GameManager");
	return nullptr;
}

void SceneManager::removeScene(const std::shared_ptr<Scene>& scene) {
	auto it = std::find(_scenes.begin(), _scenes.end(), scene);
	if (it == _scenes.end()) return;

	size_t index = std::distance(_scenes.begin(), it);
	_scenes.erase(it);

	if (_indexActiveScene == index) {
		_indexActiveScene = _scenes.empty() ? 0 : std::min(index, _scenes.size() - 1);
	}
	else if (index < _indexActiveScene) {
		--_indexActiveScene;
	}
}