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

	Mxm::Vec3 _gravity{0.0f, -9.81f, 0.0f};
	Mxm::Vec3 _velocity{};

	Mxm::Vec3 _angularVelocity{};
	Mxm::Vec3 _momentOfInertia{ 1.0f };

	float _mass = 1.0f;

	float _linearDamping{};
	float _airDamping{};
	float _angularDamping{};

	bool _isCollision{};
	Callback _collisionCallback;
public:
	static void resolveCollision(const std::shared_ptr<RigidBody>& object, const std::shared_ptr<GameObject>& other, const CollisionResult& result);

	void addImpulse(const Mxm::Vec3& vec) noexcept;
	void addAngularImpulse(const Mxm::Vec3& vec) noexcept;

	void setGravity(const Mxm::Vec3& vec) noexcept;
	const Mxm::Vec3& getGravity() const noexcept;

	void setVelocity(const Mxm::Vec3& vec) noexcept;
	const Mxm::Vec3& getVelocity() const noexcept;

	void setAngularVelocity(const Mxm::Vec3& vec) noexcept;
	const Mxm::Vec3& getAngularVelocity() const noexcept;

	void setMomentOfInertia(const Mxm::Vec3& vec) noexcept;
	const Mxm::Vec3& getMomentOfInertia() const noexcept;

	void setMass(float value) noexcept;
	float getMass() const noexcept;

	void setLinearDamping(float value) noexcept;
	float getLinearDamping() const noexcept;

	void setAirDamping(float value) noexcept;
	float getAirDamping() const noexcept;

	void setAngularDamping(float value) noexcept;
	float getAngularDamping() const noexcept;

	bool isCollision() const noexcept { return _isCollision; }

	void setCollisionCallback(const Callback& func);

	void updatePhysics();

	RigidBody() = default;
};

#endif // !RIGIDBODY_H