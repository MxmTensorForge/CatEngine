#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../Component.h"
#include "../../Physics/CollisionResult.h"

#include <vector>
#include <memory>
#include <deque>
#include <utility>
#include <functional>

class RigidBody final : public Component
{
private:
	using Callback = std::function<void(std::shared_ptr<GameObject>, const CollisionResult&)>;

	Mxm::Vec3 _acceleration{};
	Mxm::Vec3 _velocity{};

	float _friction{};
	float _airFriction{};

	bool _isCollision{};
	Callback _collisionCallback;
public:
	static void resolveCollision(const std::shared_ptr<RigidBody>& object, const std::shared_ptr<GameObject>& other, const CollisionResult& result);

	void setAcceleration(const Mxm::Vec3& vec) noexcept { _acceleration = vec; }
	void addForce(const Mxm::Vec3& vec) noexcept { _velocity += vec; }

	void setVelocity(const Mxm::Vec3& vec) noexcept { _velocity = vec; }
	const Mxm::Vec3& getVelocity() const noexcept { return _velocity; }

	void setFriction(float value) noexcept { _friction = value; }
	float getFriction() const noexcept { return _friction; }
	void setAirFriction(float value) noexcept { _airFriction = value; }
	float getAirFriction() const noexcept { return _airFriction; }

	bool isCollision() const noexcept { return _isCollision; }

	void setCollisionCallback(const Callback& func);

	void updatePhysics();

	RigidBody() = default;
};

#endif // !RIGIDBODY_H