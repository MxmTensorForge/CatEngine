#include "RigidBody.h"
#include "MeshComponent.h"

#include "../Object/Transform.h"
#include "../Object/GameObject.h"

#include "../../Core/Logger.h"
#include "../../Core/Time.h"
#include "../../Core/EngineConsts.h"

#include <vector>
#include <array>

void RigidBody::resolveCollision(const std::shared_ptr<RigidBody>& object, const std::shared_ptr<GameObject>& other, const CollisionResult& result) {
	auto move = result.normal * result.depth;
	object->getObject()->transform().translate(-move);

	auto projectVelocity = [&](const std::shared_ptr<RigidBody>& obj) {
		Mxm::Vec3 vel = obj->getVelocity();
		Mxm::Vec3 normalVel = result.normal * result.normal.dot(vel);

		obj->setVelocity(vel - normalVel);
		obj->_isCollision = true;
		};

	projectVelocity(object);

	if (object->_collisionCallback) object->_collisionCallback(other, result);

	if (result.depth > 0.5f) {
		Logger::getInstance().log(LogType::Message, "COLLISION PENEPRATION > 0.5");
	}
}

void RigidBody::updatePhysics() {
	_velocity += _acceleration * Time::fixedDeltaTime();
	getObject()->transform().translate(_velocity * Time::fixedDeltaTime());

	float friction = _isCollision ? _friction : _airFriction;
	Mxm::Vec3 frictionForce = -_velocity * friction;
	_velocity += frictionForce * Time::fixedDeltaTime();

	_isCollision = false;
}

void RigidBody::setCollisionCallback(const Callback& func) {
	_collisionCallback = func;
}