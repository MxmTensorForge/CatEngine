#include "Scene.h"

#include "Components/Camera.h"
#include "Components/Collider.h"
#include "Components/MeshComponent.h"

#include "../Physics/PhysicsSystem.h"
#include "../Core/Logger.h"

#include <iostream>

void Scene::updateCollisions() {
    _cachedObjects.clear();

    for (auto& obj : _gameObjects) {
        if (!obj->getActive()) continue;

        auto collider = obj->getComponent<Collider>();
        if (!collider) continue;

        auto rigidBody = obj->getComponent<RigidBody>();
        _cachedObjects.push_back({ obj, collider, rigidBody });
    }

    auto prevsTriggerObjects = _currentTriggerObjects;
    _currentTriggerObjects.clear();
    
    for (size_t i = 0; i < _cachedObjects.size(); ++i) {
        auto& obj1 = _cachedObjects[i];

        for (size_t j = i + 1; j < _cachedObjects.size(); ++j) {
            auto& obj2 = _cachedObjects[j];

            if (!obj1.rigidBody && !obj2.rigidBody) continue;

            bool obj1IsTrigger = obj1.collider->isTrigger();
            bool obj2IsTrigger = obj2.collider->isTrigger();

            if (obj1IsTrigger && obj2IsTrigger) continue;

            if (!Collider::checkAABB(*obj1.collider, *obj2.collider)) continue;

            auto collision = PhysicsSystem::getInstance().gjkCollision(obj1.collider, obj2.collider);
            if (!collision.first) continue;

            if (!obj1IsTrigger && !obj2IsTrigger) {
                CollisionResult result = PhysicsSystem::getInstance().epaAlgorithm(obj1.collider, obj2.collider, collision.second);

                if (obj1.rigidBody) {
                    RigidBody::resolveCollision(obj1.rigidBody, _gameObjects[j], result);
                    obj1.collider->collisionOnCallback(obj2.gameObject, result);
                }
                if (obj2.rigidBody) {
                    CollisionResult invertedResult = result;
                    invertedResult.normal = -result.normal;

                    RigidBody::resolveCollision(obj2.rigidBody, _gameObjects[i], invertedResult);
                    obj2.collider->collisionOnCallback(obj1.gameObject, result);
                }
            }

            if (obj1IsTrigger) _currentTriggerObjects[obj1.collider].insert(obj2.gameObject);
            if (obj2IsTrigger) _currentTriggerObjects[obj2.collider].insert(obj1.gameObject);
        }
    }


    for (auto& [collider, newObjs] : _currentTriggerObjects) {
        for (auto& obj : newObjs) {
            if (!prevsTriggerObjects.count(collider)) {
                collider->triggerOnCallback(obj);
            }

            collider->triggerStayCallback(obj);
        }
    }

    for (auto& [collider, prevObjs] : prevsTriggerObjects) {
        for (auto& obj : prevObjs) {
            if (!_currentTriggerObjects.count(collider)) {
                collider->triggerExitCallback(obj);
            }
        }
    }
}

std::shared_ptr<GameObject> Scene::createObject(const std::string& name, const std::string& tag) {
	auto obj = std::make_shared<GameObject>(name, tag);
	_gameObjects.push_back(obj);
	return obj;
}

std::unordered_set<std::shared_ptr<GameObject>> Scene::getObjectsWithName(const std::string& name) const {
    std::unordered_set<std::shared_ptr<GameObject>> result;

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

std::unordered_set<std::shared_ptr<GameObject>> Scene::getObjectsWithTag(const std::string& tag) const {
    std::unordered_set<std::shared_ptr<GameObject>> result;

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
        if (obj->getActive()) {
            if (auto comp = obj->getComponent<RigidBody>()) {
                comp->updatePhysics();
            }
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

bool Scene::rayCast(const Mxm::Vec3& origin, const Mxm::Vec3& dir, IntersectionInfo& out, const std::unordered_set<std::string>& tags) const {
    bool hit = false;
    float closestDistance = std::numeric_limits<float>::max();
    IntersectionInfo temp;

    for (const auto& obj : _gameObjects) {
        auto mesh_ptr = obj->getComponent<MeshComponent>();
        auto collider_ptr = obj->getComponent<Collider>();
        if (!mesh_ptr || !collider_ptr) continue;

        if (std::find(tags.begin(), tags.end(), obj->getTag()) == tags.end()) continue;

        Mxm::Mat4 inverseModel = obj->transform().getInverseWorldMatrix();
        Mxm::Vec3 invOrigin = (inverseModel * Mxm::Vec4(origin, 1.0f)).toVec3();
        Mxm::Vec3 invDir = (inverseModel * Mxm::Vec4(dir, 0.0f)).toVec3();

        if (!collider_ptr->getLocalAABB().isIntersection(invOrigin, invDir)) continue;

        if (mesh_ptr->intersection(invOrigin, invDir, temp)) {
            if (temp.distance < closestDistance) {
                closestDistance = temp.distance;
                out = temp;
                hit = true;
            }
        }
    }

    return hit;
}