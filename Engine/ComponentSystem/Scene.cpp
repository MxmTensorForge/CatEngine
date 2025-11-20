#include "Scene.h"

#include "Components/Camera.h"
#include "Components/Collider.h"
#include "Components/MeshComponent.h"

#include "../Physics/PhysicsSystem.h"

void Scene::updateCollisions() {
    for (size_t i = 0; i < _gameObjects.size(); ++i) {
        auto collider1 = _gameObjects[i]->getComponent<Collider>();
        if (!collider1) continue;

        for (size_t j = i + 1; j < _gameObjects.size(); ++j) {
            auto collider2 = _gameObjects[j]->getComponent<Collider>();
            if (!collider2) continue;

            auto rigidBody1 = _gameObjects[i]->getComponent<RigidBody>();
            auto rigidBody2 = _gameObjects[j]->getComponent<RigidBody>();
            if (!rigidBody1 && !rigidBody2) continue;

            if (!Collider::checkAABB(*collider1, *collider2)) continue;

            auto collision = PhysicsSystem::gjkCollision(collider1, collider2);
            if (!collision.first) continue;

            CollisionResult result = PhysicsSystem::epaAlgorithm(collider1, collider2, collision.second);

            if (rigidBody1 && rigidBody1->getPushable()) {
                RigidBody::resolveCollision(rigidBody1, _gameObjects[j], result);
            }
            if (rigidBody2 && rigidBody2->getPushable()) {
                CollisionResult invertedResult = result;
                invertedResult.normal = -result.normal;
                RigidBody::resolveCollision(rigidBody2, _gameObjects[i], invertedResult);
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
const std::shared_ptr<GameObject>& Scene::getMainCamera() const {
	return _mainCamera;
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