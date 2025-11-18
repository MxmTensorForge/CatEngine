#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"

class SceneManager final
{
private:
	std::vector<std::shared_ptr<Scene>> _scenes;
	size_t _indexActiveScene = 0;
public:
	std::shared_ptr<Scene> createScene();
	void setActiveScene(const std::shared_ptr<Scene>& scene);

	std::shared_ptr<Scene> getActiveScene() const;

	void removeScene(const std::shared_ptr<Scene>& scene);
};

#endif // !SCENEMANAGER_H
