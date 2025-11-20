#ifndef SCENE_H
#define SCENE_H

#include "Object/GameObject.h"
#include "Components/MeshComponent.h"

#include <set>

class Scene final
{
private:
	std::vector<std::shared_ptr<GameObject>> _gameObjects;
	std::shared_ptr<GameObject> _mainCamera{};

	void updateRecursive(Transform* transform);
public:
	Scene() = default;
	~Scene() = default;

	std::shared_ptr<GameObject> createObject(const std::string& name, const std::string& tag = "default");

	std::set<std::shared_ptr<GameObject>> getObjectsWithName(const std::string& tag) const;
	std::shared_ptr<GameObject> getFirstObjectWithName(const std::string& tag) const;

	std::set<std::shared_ptr<GameObject>> getObjectsWithTag(const std::string& tag) const;
	std::shared_ptr<GameObject> getFirstObjectWithTag(const std::string& tag) const;

	void removeObject(const std::shared_ptr<GameObject>& obj);
	void removeObjectsWithTag(const std::string& tag);
	void removeObjectsWithName(const std::string& name);

	void start();
	void update();
	void updatePhysics();
	void updateCollisions();

	const std::vector<std::shared_ptr<GameObject>>& getGameObjects() const noexcept;
	void clear();

	void setMainCamera(const std::shared_ptr<GameObject>& camera);
	const std::shared_ptr<GameObject>& getMainCamera() const;

	bool rayCast(const Mxm::Vec3& origin, const Mxm::Vec3& dir, IntersectionInfo& out, const std::set<std::string>& tags) const;
};

#endif // !SCENE_H
