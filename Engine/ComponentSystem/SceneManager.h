#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"

#include <memory>

class SceneManager final
{
private:
	std::vector<std::unique_ptr<Scene>> _scenes;
	Scene* _pendingActiveScene = nullptr;
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

	Scene* createScene();
	Scene* getActiveScene() const;

	void setActiveScene(Scene* scene);
	void removeScene(Scene* scene);
};

#endif // !SCENEMANAGER_H
