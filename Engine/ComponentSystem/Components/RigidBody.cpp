#include "RigidBody.h"
#include "MeshComponent.h"

#include "../Object/Transform.h"
#include "../Object/GameObject.h"

#include "../../Core/Logger.h"
#include "../../Core/Time.h"
#include "../../Core/EngineConsts.h"

#include <vector>
#include <array>

void RigidBody::resolveCollision(RigidBody* object, const std::shared_ptr<GameObject>& other, const CollisionResult& result) {
	auto move = result.normal * result.depth;
	object->getObject()->transform().translate(-move);

	auto projectVelocity = [&](RigidBody* obj) {
		Mxm::Vec3 vel = obj->getVelocity();
		Mxm::Vec3 normalVel = result.normal * result.normal.dot(vel);

		obj->setVelocity(vel - normalVel);
		obj->_isCollision = true;
		};

	projectVelocity(object);

	if (result.depth > 0.3f) {
		Logger::getInstance().log(LogType::Message, "COLLISION PENEPRATION > 0.3");
	}
}

void RigidBody::updatePhysics() {
	float damping = _isCollision ? _linearDamping : _airDamping;

	Mxm::Vec3 acceleration = _gravity;
	_velocity += acceleration * Time::fixedDeltaTime();

	_velocity *= 1.0f - damping * Time::fixedDeltaTime();
	_angularVelocity *= 1.0f - _angularDamping * Time::fixedDeltaTime();

	getObject()->transform().translate(_velocity * Time::fixedDeltaTime());
	getObject()->transform().rotate(_angularVelocity * Time::fixedDeltaTime());

	_isCollision = false;
}


void RigidBody::addImpulse(const Mxm::Vec3& vec) noexcept { _velocity += vec / _mass; }
void RigidBody::addAngularImpulse(const Mxm::Vec3& vec) noexcept { _angularVelocity += vec / _momentOfInertia; }

void RigidBody::setGravity(const Mxm::Vec3& vec) noexcept { _gravity = vec; }
const Mxm::Vec3& RigidBody::getGravity() const noexcept { return _gravity; }

void RigidBody::setVelocity(const Mxm::Vec3& vec) noexcept { _velocity = vec; }
const Mxm::Vec3& RigidBody::getVelocity() const noexcept { return _velocity; }

void RigidBody::setAngularVelocity(const Mxm::Vec3& vec) noexcept { _angularVelocity = vec; }
const Mxm::Vec3& RigidBody::getAngularVelocity() const noexcept { return _angularVelocity; }

void RigidBody::setMomentOfInertia(const Mxm::Vec3& vec) noexcept { _momentOfInertia = vec; }
const Mxm::Vec3& RigidBody::getMomentOfInertia() const noexcept { return _momentOfInertia; }

void RigidBody::setMass(float value) noexcept { _mass = value; }
float RigidBody::getMass() const noexcept { return _mass; }

void RigidBody::setLinearDamping(float value) noexcept { _linearDamping = value; }
float RigidBody::getLinearDamping() const noexcept { return _linearDamping; }

void RigidBody::setAirDamping(float value) noexcept { _airDamping = value; }
float RigidBody::getAirDamping() const noexcept { return _airDamping; }

void RigidBody::setAngularDamping(float value) noexcept { _angularDamping = value; }
float RigidBody::getAngularDamping() const noexcept { return _angularDamping; }