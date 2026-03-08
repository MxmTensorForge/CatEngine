#include "Scene.h"

#include "Components/Camera.h"
#include "Components/Collider.h"
#include "Components/MeshComponent.h"
#include "Components/RigidBody.h"

#include "../Physics/PhysicsSystem.h"
#include "../Core/Logger.h"
#include "../Core/EventSystem.h"

void Scene::processCollisionPair(const CachedObject& obj1, const CachedObject& obj2) {
    auto gameObject1 = obj1.gameObject;
    auto collider1 = obj1.collider;
    auto rigidbody1 = obj1.rigidBody;

    auto gameObject2 = obj2.gameObject;
    auto collider2 = obj2.collider;
    auto rigidbody2 = obj2.rigidBody;

    if (!gameObject1 || !collider1 || !gameObject1->getActive()) return;
    if (!rigidbody1 && !rigidbody2) return;

    bool obj1IsTrigger = collider1->isTrigger();
    bool obj2IsTrigger = collider2->isTrigger();

    if (obj1IsTrigger && obj2IsTrigger) return;

    if (!Collider::checkAABB(*collider1, *collider2)) return;

    auto collision = PhysicsSystem::getInstance().gjkCollision(collider1, collider2);
    if (!collision.first) return;

    if (!obj1IsTrigger && !obj2IsTrigger) {
        CollisionResult result = PhysicsSystem::getInstance().epaAlgorithm(collider1, collider2, collision.second);

        if (rigidbody1 && !rigidbody2) {
            PhysicsSystem::getInstance().resolveCollisionStatic(rigidbody1, result);
            collider1->collisionOnCallback(gameObject2, result);
        }
        else if (rigidbody2 && !rigidbody1) {
            result.normal = -result.normal;

            PhysicsSystem::getInstance().resolveCollisionStatic(rigidbody2, result);
            collider2->collisionOnCallback(gameObject1, result);
        }
        else if (rigidbody1 && rigidbody2) {
            PhysicsSystem::getInstance().resolveCollisionDynamic(rigidbody1, rigidbody2, result);

            collider1->collisionOnCallback(gameObject2, result);
            result.normal = -result.normal;
            collider2->collisionOnCallback(gameObject1, result);
        }
    }

    if (obj1IsTrigger) _currentTriggerObjects[collider1].insert(gameObject2);
    if (obj2IsTrigger) _currentTriggerObjects[collider2].insert(gameObject1);
}
void Scene::processTriggers(const std::unordered_map<Collider*, std::unordered_set<GameObject*>>& prevsTriggerObjects) {
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

void Scene::updateCollisions() {
    _cachedObjects.clear();

    for (auto& obj : _gameObjects) {
        if (!obj->getActive()) continue;

        auto collider = obj->getComponent<Collider>();
        if (!collider) continue;

        auto rigidBody = obj->getComponent<RigidBody>();
        _cachedObjects.push_back({ obj.get(), collider, rigidBody});
    }

    auto prevsTriggerObjects = std::move(_currentTriggerObjects);
    _currentTriggerObjects.clear();
    
    for (size_t i = 0; i < _cachedObjects.size(); ++i) {
        for (size_t j = i + 1; j < _cachedObjects.size(); ++j) {
            processCollisionPair(_cachedObjects[i], _cachedObjects[j]);
        }
    }

    processTriggers(prevsTriggerObjects);
}

GameObject* Scene::createObject(const std::string& name, const std::string& tag) {
    _gameObjects.push_back(std::make_unique<GameObject>(name, tag));
    return _gameObjects.back().get();
}

std::unordered_set<GameObject*> Scene::getObjectsWithName(const std::string& name) const {
    std::unordered_set<GameObject*> result;

    for (const auto& obj : _gameObjects) {
        if (obj->getName() == name) {
            result.insert(obj.get());
        }
    }

    return result;
}
GameObject* Scene::getFirstObjectWithName(const std::string& name) const {
    auto objects = getObjectsWithName(name);
    return objects.empty() ? nullptr : *objects.begin();
}

std::unordered_set<GameObject*> Scene::getObjectsWithTag(const std::string& tag) const {
    std::unordered_set<GameObject*> result;

    for (const auto& obj : _gameObjects) {
        if (obj->getTag() == tag) {
            result.insert(obj.get());
        }
    }

    return result;
}
GameObject* Scene::getFirstObjectWithTag(const std::string& tag) const {
    auto objects = getObjectsWithTag(tag);
    return objects.empty() ? nullptr : *objects.begin();
}

const std::vector<PointLight*>& Scene::getPointLights() const {
    return _pointLightsCache;
}
const DirectionLight* Scene::getDirectionLight() const {
    return _dirLightCache;
}
void Scene::updateLightCache() const {
    _pointLightsCache.clear();
    _dirLightCache = nullptr;

    for (const auto& obj : _gameObjects) {
        if (auto light = obj->getComponent<PointLight>()) {
            _pointLightsCache.push_back(light);
        }
        if (auto dirLight = obj->getComponent<DirectionLight>()) {
            _dirLightCache = dirLight;
        }
    }
}

void Scene::removeObject(const GameObject* obj) {
    _gameObjects.erase(
        std::remove_if(_gameObjects.begin(), _gameObjects.end(),
            [&obj](const std::unique_ptr<GameObject>& item) {
                return item.get() == obj;
            }),
        _gameObjects.end()
    );
}
void Scene::removeObjectsWithTag(const std::string& tag) {
    _gameObjects.erase(std::remove_if(_gameObjects.begin(), _gameObjects.end(), [&tag](const std::unique_ptr<GameObject>& obj) {
        return obj->getTag() == tag;
        }), _gameObjects.end());
}
void Scene::removeObjectsWithName(const std::string& name) {
    _gameObjects.erase(std::remove_if(_gameObjects.begin(), _gameObjects.end(), [&name](const std::unique_ptr<GameObject>& obj) {
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
    updateLightCache();
}
void Scene::update() {
	for (auto& obj : _gameObjects) {
        if (!obj->getActive() || obj->transform().getParent()) continue;
        updateRecursive(&obj->transform());
	}
    if (EventSystem::getInstance().poll("light_update")) updateLightCache();
}
void Scene::fixedUpdate() {
    for (auto& obj : _gameObjects) {
        if (!obj->getActive()) continue;
        obj->fixedUpdateComponents();
    }
    updateCollisions();
}

void Scene::updateAnimator() {
    _animator.update();
}

std::vector<GameObject*> Scene::getGameObjects() const noexcept {
    std::vector<GameObject*> result;
    for (const auto& obj : _gameObjects) {
        result.push_back(obj.get());
    }
    return result;
}
void Scene::clear() {
    _gameObjects.clear();
    _cachedObjects.clear();
    _currentTriggerObjects.clear();
    _mainCamera = nullptr;
}

void Scene::setMainCamera(GameObject* camera) {
	if (camera->hasComponent<Camera>()) _mainCamera = camera;
}
GameObject* Scene::getMainCamera() const {
    return _mainCamera;
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

void Scene::setAmbientColor(Color color) noexcept {
    _ambientColor = color;
}
void Scene::setBackgroundColor(Color color) noexcept {
    _backgroundColor = color;
}

Color Scene::getAmbientColor() const noexcept {
    return _ambientColor;
}
Color Scene::getBackgroundColor() const noexcept {
    return _backgroundColor;
}