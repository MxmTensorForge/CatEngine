#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"

class SceneManager final
{
private:
	std::vector<std::shared_ptr<Scene>> _scenes;
	std::shared_ptr<Scene> _pendingActiveScene = nullptr;
	size_t _indexActiveScene = 0;

	SceneManager() = default;
	~SceneManager() = default;

	void processPendingScene();
	bool hasPendingScene();
	friend class Application;
public:
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;

	static SceneManager& getInstance() {
		static SceneManager manager;
		return manager;
	}

	std::shared_ptr<Scene> createScene();

	std::shared_ptr<Scene> getActiveScene() const;
	void setActiveScene(const std::shared_ptr<Scene>& scene);

	void removeScene(const std::shared_ptr<Scene>& scene);
};

#endif // !SCENEMANAGER_H
