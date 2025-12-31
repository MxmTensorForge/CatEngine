#ifndef SCENE_H
#define SCENE_H

#include "Object/GameObject.h"
#include "Components/MeshComponent.h"
#include "../Animation/Animator.h"

#include "Components/PointLight.h"
#include "Components/DirectionLight.h"

#include <unordered_set>
#include <unordered_map>

class Collider;
class RigidBody;

class Scene final
{
private:
	std::vector<std::shared_ptr<GameObject>> _gameObjects;
	std::weak_ptr<GameObject> _mainCamera;
	Animator _animator;

	struct CachedObject {
		std::weak_ptr<GameObject> gameObject;
		std::weak_ptr<Collider> collider;
		std::weak_ptr<RigidBody> rigidBody;
	};
	std::vector<CachedObject> _cachedObjects;

	std::unordered_map<std::shared_ptr<Collider>, std::unordered_set<std::shared_ptr<GameObject>>> _currentTriggerObjects;

	mutable std::vector<std::shared_ptr<PointLight>> _pointLightsCache;
	mutable std::shared_ptr<DirectionLight> _dirLightCache;
	mutable bool _cacheValid = false;

	void updateRecursive(Transform* transform);
public:
	Scene() = default;
	~Scene() = default;

	std::shared_ptr<GameObject> createObject(const std::string& name, const std::string& tag = "default");

	std::unordered_set<std::shared_ptr<GameObject>> getObjectsWithName(const std::string& name) const;
	std::shared_ptr<GameObject> getFirstObjectWithName(const std::string& name) const;

	std::unordered_set<std::shared_ptr<GameObject>> getObjectsWithTag(const std::string& tag) const;
	std::shared_ptr<GameObject> getFirstObjectWithTag(const std::string& tag) const;

	const std::vector<std::shared_ptr<PointLight>>& getPointLights() const;
	const std::shared_ptr<DirectionLight>& getDirectionLight() const;
	void updateLightCache() const;

	void removeObject(const std::shared_ptr<GameObject>& obj);
	void removeObjectsWithTag(const std::string& tag);
	void removeObjectsWithName(const std::string& name);

	void start();
	void update();
	void updateAnimator();
	void updatePhysics();
	void updateCollisions();

	inline Animator& getAnimator() noexcept { return _animator; }

	const std::vector<std::shared_ptr<GameObject>>& getGameObjects() const noexcept;
	void clear();

	void setMainCamera(const std::shared_ptr<GameObject>& camera);
	std::shared_ptr<GameObject> getMainCamera() const;

	bool rayCast(const Mxm::Vec3& origin, const Mxm::Vec3& dir, IntersectionInfo& out, const std::unordered_set<std::string>& tags) const;
};

#endif // !SCENE_H
