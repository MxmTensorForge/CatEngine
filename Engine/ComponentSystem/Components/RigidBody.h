#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../Component.h"

#include "../../Mxm/Mat4.h"
#include "../../Geometry/Triangle.h"
#include <vector>
#include <memory>
#include <deque>
#include <utility>
#include <functional>

struct CollisionResult {
	const Mxm::Vec3 normal;
	const float depth;
};

class RigidBody final : public Component
{
private:
	struct Edge
	{
		Mxm::Vec3 a, b;
		Edge reversed() const { return Edge{ b, a }; }

		bool operator==(const Edge& other) const {
			return (a == other.a && b == other.b);
		}
	};

	//optimization
	std::vector<Edge> _uniqueEdges;
	std::vector<Triangle> _polytope;
	std::deque<Mxm::Vec3> _simplex;

	Mxm::Vec3 furthestPoint(const Mxm::Vec3& dir);
	Mxm::Vec3 minkowskiDifference(const std::shared_ptr<RigidBody>& object, const Mxm::Vec3& dir);
	bool handleSimplex(std::deque<Mxm::Vec3>& simplex, Mxm::Vec3& direction);

	std::pair<Triangle, float> findClosestFace(const std::vector<Triangle>& polytope);
	void expandPolytope(std::vector<Triangle>& polytope, const Mxm::Vec3& newPoint);

	Mxm::Vec3 _acceleration{};
	Mxm::Vec3 _velocity{};

	bool _isDynamic = true;
	float _friction{};
	float _airFriction{};

	bool _isCollision{};
	std::function<void(std::shared_ptr<RigidBody>, const CollisionResult&)> _collisionCallback;
public:
	std::pair<bool, std::deque<Mxm::Vec3>> gjkCollision(const std::shared_ptr<RigidBody>& object);
	CollisionResult simplifiedEPA(const std::shared_ptr<RigidBody>& object, const std::deque<Mxm::Vec3>& simplex);
	
	static void resolveCollision(const CollisionResult& result, const std::shared_ptr<RigidBody>& object1, const std::shared_ptr<RigidBody>& object2);

	void setAcceleration(const Mxm::Vec3& vec) noexcept { _acceleration = vec; }
	void addForce(const Mxm::Vec3& vec) noexcept { _velocity += vec; }

	void setVelocity(const Mxm::Vec3& vec) noexcept { _velocity = vec; }
	const Mxm::Vec3& getVelocity() const noexcept { return _velocity; }

	void setDynamic(bool state) noexcept { _isDynamic = state; }
	bool getDynamicState() const noexcept { return _isDynamic; }

	void setFriction(float value) noexcept { _friction = value; }
	float getFriction() const noexcept { return _friction; }
	void setAirFriction(float value) noexcept { _airFriction = value; }
	float getAirFriction() const noexcept { return _airFriction; }

	bool isCollision() const noexcept { return _isCollision; }

	void setCollisionCallback(const std::function<void(std::shared_ptr<RigidBody>, const CollisionResult&)>& func);

	void updatePhysics();

	RigidBody() = default;
};

#endif // !RIGIDBODY_H