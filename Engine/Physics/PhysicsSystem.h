#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "../ComponentSystem/Components/Collider.h"
#include "../ComponentSystem/Components/RigidBody.h"
#include "../Geometry/Triangle.h"

#include "CollisionResult.h"

#include <utility>
#include <deque>
#include <memory>

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
	static std::vector<Edge> _uniqueEdges;
	static std::vector<Triangle> _polytope;
	static std::deque<Mxm::Vec3> _simplex;

	static Mxm::Vec3 furthestPoint(const std::shared_ptr<Collider>& collider, const Mxm::Vec3& dir);
	static Mxm::Vec3 minkowskiDifference(const std::shared_ptr<Collider>& collider1, const std::shared_ptr<Collider>& collider2, const Mxm::Vec3& dir);
	static bool handleSimplex(std::deque<Mxm::Vec3>& simplex, Mxm::Vec3& direction);

	static std::pair<Triangle, float> findClosestFace(const std::vector<Triangle>& polytope);
	static void expandPolytope(std::vector<Triangle>& polytope, const Mxm::Vec3& newPoint);
public:
	PhysicsSystem() = delete;
	~PhysicsSystem() = delete;

	static std::pair<bool, std::deque<Mxm::Vec3>> gjkCollision(const std::shared_ptr<Collider>& collider1, const std::shared_ptr<Collider>& collider2);
	static CollisionResult epaAlgorithm(const std::shared_ptr<Collider>& collider1, const std::shared_ptr<Collider>& collider2, const std::deque<Mxm::Vec3>& simplex);
};

#endif // !PHYSICSSYSTEM_H