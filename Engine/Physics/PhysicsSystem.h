#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "../Geometry/Triangle.h"
#include "CollisionResult.h"

#include <utility>
#include <deque>
#include <vector>

class Collider;
class RigidBody;

class PhysicsSystem final
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
	// *** Optimization ***
	std::vector<Edge> _uniqueEdges;
	std::vector<Triangle> _polytope;
	std::deque<Mxm::Vec3> _simplex;

	Mxm::Vec3 furthestPoint(const Collider* collider, const Mxm::Vec3& dir);
	Mxm::Vec3 minkowskiDifference(const Collider* collider1, const Collider* collider2, const Mxm::Vec3& dir);
	bool handleSimplex(std::deque<Mxm::Vec3>& simplex, Mxm::Vec3& direction);

	std::pair<Triangle, float> findClosestFace(const std::vector<Triangle>& polytope);
	void expandPolytope(std::vector<Triangle>& polytope, const Mxm::Vec3& newPoint);

	PhysicsSystem() = default;
public:
	~PhysicsSystem() = default;

	PhysicsSystem(const PhysicsSystem&) = delete;
	PhysicsSystem& operator=(const PhysicsSystem&) = delete;
	PhysicsSystem(PhysicsSystem&&) = delete;
	PhysicsSystem& operator=(PhysicsSystem&&) = delete;

	static PhysicsSystem& getInstance() noexcept {
		static PhysicsSystem sys;
		return sys;
	}

	std::pair<bool, std::deque<Mxm::Vec3>> gjkCollision(const Collider* collider1, const Collider* collider2);
	CollisionResult epaAlgorithm(const Collider* collider1, const Collider* collider2, const std::deque<Mxm::Vec3>& simplex);

	void resolveCollisionStatic(RigidBody* rb, const CollisionResult& result);
	void resolveCollisionDynamic(RigidBody* rb1, RigidBody* rb2, const CollisionResult& result);
};

#endif // !PHYSICSSYSTEM_H