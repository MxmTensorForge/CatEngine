#include "Scene.h"
#include "Components/Camera.h"
#include "Components/RigidBody.h"
#include "Components/MeshComponent.h"

void Scene::updateCollisions() {
    for (size_t i = 0; i < _gameObjects.size(); ++i) {
        auto rigidBody1 = _gameObjects[i]->getComponent<RigidBody>();
        if (!rigidBody1) continue;

        for (size_t j = i + 1; j < _gameObjects.size(); ++j) {
            auto rigidBody2 = _gameObjects[j]->getComponent<RigidBody>();
            if (!rigidBody2) continue;

            if (!rigidBody1->getDynamicState() && !rigidBody2->getDynamicState()) continue;

            auto collision = rigidBody1->gjkCollision(rigidBody2);
            if (!collision.first) continue;

            CollisionResult result = rigidBody1->simplifiedEPA(rigidBody2, collision.second);
            RigidBody::resolveCollision(result, rigidBody1, rigidBody2);
        }
    }
}

std::shared_ptr<GameObject> Scene::createObject(const std::string& tag) {
	auto obj = std::make_shared<GameObject>(tag);
	_gameObjects.push_back(obj);
	return obj;
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
	_gameObjects.erase(std::remove(_gameObjects.begin(), _gameObjects.end(), obj), _gameObjects.end());
}
void Scene::removeObject(const std::string& tag) {
    _gameObjects.erase(std::remove_if(_gameObjects.begin(), _gameObjects.end(), [&tag](const std::shared_ptr<GameObject>& obj) {
        return obj->getTag() == tag;
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