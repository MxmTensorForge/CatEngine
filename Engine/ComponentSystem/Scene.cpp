#include "Scene.h"

#include "Components/Camera.h"
#include "Components/Collider.h"
#include "Components/MeshComponent.h"

#include "../Physics/PhysicsSystem.h"

#include <iostream>

void Scene::updateCollisions() {
    struct CachedObject {
        std::shared_ptr<GameObject> gameObject;
        std::shared_ptr<Collider> collider;
        std::shared_ptr<RigidBody> rigidBody;
    };
    std::vector<CachedObject> cachedObjects;
    cachedObjects.reserve(_gameObjects.size());

    for (auto& obj : _gameObjects) {
        auto collider = obj->getComponent<Collider>();
        if (!collider) continue;

        auto rigidBody = obj->getComponent<RigidBody>();
        cachedObjects.push_back({ obj, collider, rigidBody });
    }
    
    for (size_t i = 0; i < cachedObjects.size(); ++i) {
        auto& obj1 = cachedObjects[i];

        for (size_t j = i + 1; j < cachedObjects.size(); ++j) {
            auto& obj2 = cachedObjects[j];

            if (!obj1.rigidBody && !obj2.rigidBody) continue;

            if (!Collider::checkAABB(*obj1.collider, *obj2.collider)) continue;

            auto collision = PhysicsSystem::getInstance().gjkCollision(obj1.collider, obj2.collider);
            if (!collision.first) continue;

            CollisionResult result = PhysicsSystem::getInstance().epaAlgorithm(obj1.collider, obj2.collider, collision.second);

            if (obj1.rigidBody && obj1.rigidBody->getPushable()) {
                RigidBody::resolveCollision(obj1.rigidBody, _gameObjects[j], result);
            }
            if (obj2.rigidBody && obj2.rigidBody->getPushable()) {
                CollisionResult invertedResult = result;
                invertedResult.normal = -result.normal;
                RigidBody::resolveCollision(obj2.rigidBody, _gameObjects[i], invertedResult);
            }
        }
    }
}

std::shared_ptr<GameObject> Scene::createObject(const std::string& name, const std::string& tag) {
	auto obj = std::make_shared<GameObject>(name, tag);
	_gameObjects.push_back(obj);
	return obj;
}

std::set<std::shared_ptr<GameObject>> Scene::getObjectsWithName(const std::string& name) const {
    std::set<std::shared_ptr<GameObject>> result;

    for (const auto& obj : _gameObjects) {
        if (obj->getName() == name) {
            result.insert(obj);
        }
    }

    return result;
}
std::shared_ptr<GameObject> Scene::getFirstObjectWithName(const std::string& name) const {
    auto objects = getObjectsWithName(name);
    return objects.empty() ? nullptr : *objects.begin();
}

std::set<std::shared_ptr<GameObject>> Scene::getObjectsWithTag(const std::string& tag) const {
    std::set<std::shared_ptr<GameObject>> result;

    for (const auto& obj : _gameObjects) {
        if (obj->getTag() == tag) {
            result.insert(obj);
        }
    }

    return result;
}
std::shared_ptr<GameObject> Scene::getFirstObjectWithTag(const std::string& tag) const {
    auto objects = getObjectsWithTag(tag);
    return objects.empty() ? nullptr : *objects.begin();
}

void Scene::removeObject(const std::shared_ptr<GameObject>& obj) {
    _gameObjects.erase(
        std::remove_if(_gameObjects.begin(), _gameObjects.end(),
            [&obj](const std::shared_ptr<GameObject>& item) {
                return item == obj;
            }),
        _gameObjects.end()
    );
}
void Scene::removeObjectsWithTag(const std::string& tag) {
    _gameObjects.erase(std::remove_if(_gameObjects.begin(), _gameObjects.end(), [&tag](const std::shared_ptr<GameObject>& obj) {
        return obj->getTag() == tag;
        }), _gameObjects.end());
}
void Scene::removeObjectsWithName(const std::string& name) {
    _gameObjects.erase(std::remove_if(_gameObjects.begin(), _gameObjects.end(), [&name](const std::shared_ptr<GameObject>& obj) {
        return obj->getName() == name;
        }), _gameObjects.end());
}

void Scene::updateRecursive(Transform* transform) {
    if (!transform) return;

    transform->getOwner()->updateComponents();

    for (auto t : transform->getChildren()) {
        if (!t) continue;
        updateRecursive(t);
    }
}

void Scene::start() {
	for (auto& obj : _gameObjects) {
		obj->startComponents();
	}
}
void Scene::update() {
	for (auto& obj : _gameObjects) {
        if (!obj->getActive() || obj->transform().getParent()) continue;
        updateRecursive(&obj->transform());
	}
    _animator.update();
}
void Scene::updateAnimator() {
    _animator.update();
}
void Scene::updatePhysics() {
    for (auto& obj : _gameObjects) {
        if (auto comp = obj->getComponent<RigidBody>()) {
            comp->updatePhysics();
        }
    }
}

const std::vector<std::shared_ptr<GameObject>>& Scene::getGameObjects() const noexcept {
	return _gameObjects;
}
void Scene::clear() {
	_gameObjects.clear();
}

void Scene::setMainCamera(const std::shared_ptr<GameObject>& camera) {
	if (camera->hasComponent<Camera>()) _mainCamera = camera;
}
std::shared_ptr<GameObject> Scene::getMainCamera() const {
    return _mainCamera.lock();
}

bool Scene::rayCast(const Mxm::Vec3& origin, const Mxm::Vec3& dir, IntersectionInfo& out, const std::set<std::string>& tags) const {
    bool hit = false;
    float closestDistance = std::numeric_limits<float>::max();
    IntersectionInfo temp;

    for (const auto& obj : _gameObjects) {
        auto mesh_ptr = obj->getComponent<MeshComponent>();
        if (!mesh_ptr) continue;

        if (std::find(tags.begin(), tags.end(), obj->getTag()) == tags.end()) continue;

        if (mesh_ptr->intersection(origin, dir, temp)) {
            if (temp.distance < closestDistance) {
                closestDistance = temp.distance;
                out = temp;
                hit = true;
            }
        }
    }

    return hit;
}