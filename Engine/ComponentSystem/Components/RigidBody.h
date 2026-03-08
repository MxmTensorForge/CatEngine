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
	Mxm::Vec3 _gravity{0.0f, -9.81f, 0.0f};
	Mxm::Vec3 _velocity{};

	Mxm::Vec3 _angularVelocity{};
	Mxm::Vec3 _momentOfInertia{ 1.0f };

	float _mass = 1.0f;

	float _linearDamping{};
	float _airDamping{};
	float _angularDamping{};

	bool _isCollision{};
public:
	void addImpulse(const Mxm::Vec3& vec) noexcept;
	void addAngularImpulse(const Mxm::Vec3& vec) noexcept;
	void addImpulseInPoint(const Mxm::Vec3& point, Mxm::Vec3& vec) noexcept;

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
	void setIsCollision(bool state) noexcept { _isCollision = state; }

	void fixedUpdate() override;

	RigidBody() = default;
};

#endif // !RIGIDBODY_H