#include "PhysicsSystem.h"

#include "../ComponentSystem/Components/MeshComponent.h"
#include "../ComponentSystem/Object/GameObject.h"

#include "../Core/Logger.h"
#include "../Core/Time.h"
#include "../Core/EngineConsts.h"

#include <array>

std::vector<PhysicsSystem::Edge> PhysicsSystem::_uniqueEdges{};
std::vector<Triangle> PhysicsSystem::_polytope;
std::deque<Mxm::Vec3> PhysicsSystem::_simplex;

Mxm::Vec3 PhysicsSystem::furthestPoint(const std::shared_ptr<Collider>& collider, const Mxm::Vec3& dir) {
	auto& transform = collider->getObject()->transform();
	Mxm::Vec3 localDir = (transform.getWorldMatrix().transposed() * Mxm::Vec4(dir, 1.0f)).toVec3(); //UPD: We need to ignore the offset, keep the scale and invert the rotation

	float greatestDot = -std::numeric_limits<float>::max();
	Mxm::Vec3 furthestPoint{};

	for (const auto p : collider->getVertices()) {
		float currDot = localDir.dot(p);
		if (greatestDot < currDot) {
			furthestPoint = p;
			greatestDot = currDot;
		}
	}
	return (transform.getWorldMatrix() * Mxm::Vec4(furthestPoint, 1.0f)).toVec3();
}
Mxm::Vec3 PhysicsSystem::minkowskiDifference(const std::shared_ptr<Collider>& collider1, const std::shared_ptr<Collider>& collider2, const Mxm::Vec3& dir) {
	return furthestPoint(collider1, dir) - furthestPoint(collider2, -dir);
}

bool PhysicsSystem::handleSimplex(std::deque<Mxm::Vec3>& simplex, Mxm::Vec3& direction) {
	Mxm::Vec3 a = simplex[0];
	Mxm::Vec3 b = simplex[1];
	Mxm::Vec3 ao = -a;
	Mxm::Vec3 ab = b - a;

	if (simplex.size() == 2) {
		direction = ab.cross(ao).cross(ab);

		return true;
	}
	else if (simplex.size() == 3) {
		Mxm::Vec3 c = simplex[2];
		Mxm::Vec3 ac = c - a;
		Mxm::Vec3 abc = ab.cross(ac);

		Mxm::Vec3 abPerp = ab.cross(abc);
		Mxm::Vec3 acPerp = abc.cross(ac);

		if (abPerp.dot(ao) > 0.0f) {
			simplex = { a, b };
			direction = abPerp;

			return true;
		}
		else if (acPerp.dot(ao) > 0.0f) {
			simplex = { a, c };
			direction = acPerp;

			return true;
		}

		if (abc.dot(ao) > 0.0f) {
			direction = abc;
		}
		else {
			direction = -abc;
		}

		return true;
	}
	else if (simplex.size() == 4) {
		Mxm::Vec3 c = simplex[2];
		Mxm::Vec3 d = simplex[3];

		Mxm::Vec3 ac = c - a;
		Mxm::Vec3 ad = d - a;

		Mxm::Vec3 abc = ab.cross(ac);
		Mxm::Vec3 adb = ad.cross(ab);
		Mxm::Vec3 acd = ac.cross(ad);

		if (abc.dot(ao) > 0.0f) {
			simplex = { a, b, c };
			direction = abc;

			return true;
		}
		else if (adb.dot(ao) > 0.0f) {
			simplex = { a, d, b };
			direction = adb;

			return true;
		}
		else if (acd.dot(ao) > 0.0f) {
			simplex = { a, c, d };
			direction = acd;

			return true;
		}

		return false;
	}

	return true;
}

std::pair<Triangle, float> PhysicsSystem::findClosestFace(const std::vector<Triangle>& polytope) {
	Triangle closestFace = polytope[0];
	float minDist = std::numeric_limits<float>::max();

	for (const auto& c : polytope) {
		float dist = c.normal().dot(c[0].toVec3());

		if (dist < minDist) {
			minDist = dist;
			closestFace = c;
		}
	}
	return { closestFace, minDist };
}
void PhysicsSystem::expandPolytope(std::vector<Triangle>& polytope, const Mxm::Vec3& newPoint) {
	_uniqueEdges.clear();

	auto it = polytope.begin();
	while (it != polytope.end()) {
		if (it->normal().dot(newPoint) > 0.0f) {
			std::array<Edge, 3> edges = {
				Edge{(*it)[0].toVec3(), (*it)[1].toVec3()},
				Edge{(*it)[1].toVec3(), (*it)[2].toVec3()},
				Edge{(*it)[2].toVec3(), (*it)[0].toVec3()}
			};

			for (const auto& e : edges) {
				auto rev = e.reversed();
				auto found = std::find(_uniqueEdges.begin(), _uniqueEdges.end(), rev);

				if (found != _uniqueEdges.end()) {
					_uniqueEdges.erase(found);
				}
				else {
					_uniqueEdges.push_back(e);
				}
			}

			it = polytope.erase(it);
		}
		else {
			it++;
		}
	}

	for (const auto& e : _uniqueEdges) {
		_polytope.emplace_back(e.a, e.b, newPoint);
	}
}
std::pair<bool, std::deque<Mxm::Vec3>> PhysicsSystem::gjkCollision(const std::shared_ptr<Collider>& collider1, const std::shared_ptr<Collider>& collider2) {
	Mxm::Vec3 direction = Mxm::Vec3(1.0f, 0.0f, 0.0f);

	Mxm::Vec3 support = minkowskiDifference(collider1, collider2, direction);

	_simplex.clear();
	_simplex.push_front(support);

	direction = -support;

	int iters = 0;
	while (iters < 32) {
		support = minkowskiDifference(collider1, collider2, direction);

		if (direction.dot(support) < 0.0f) {
			return { false, _simplex };
		}

		_simplex.push_front(support);
		if (!handleSimplex(_simplex, direction)) {
			return { true, _simplex };
		}

		iters++;
	}
	return { false, _simplex };
}
CollisionResult PhysicsSystem::epaAlgorithm(const std::shared_ptr<Collider>& collider1, const std::shared_ptr<Collider>& collider2, const std::deque<Mxm::Vec3>& simplex) {
	_polytope.clear();
	_polytope.reserve(16);

	_polytope = {
		Triangle(simplex[0], simplex[1], simplex[2]), //ABC
		Triangle(simplex[0], simplex[3], simplex[1]), //ADB
		Triangle(simplex[0], simplex[2], simplex[3]), //ACD
		Triangle(simplex[1], simplex[3], simplex[2])  //BDC
	};

	_uniqueEdges.reserve(32);

	for (int i = 0; i < 32; i++) {
		auto [closestFace, closestDist] = findClosestFace(_polytope);
		Mxm::Vec3 newPoint = minkowskiDifference(collider1, collider2, closestFace.normal());

		//If we have reached the boundaries of the Minkowski set, then there is no point in continuing the algorithm.
		float newDist = closestFace.normal().dot(newPoint);
		if (newDist - closestDist < 0.001f) {
			return CollisionResult{ closestFace.normal(), closestDist };
		}

		expandPolytope(_polytope, newPoint);
	}
	return CollisionResult{ Mxm::Vec3(1.0f, 0.0f, 0.0f), 0.1f };
}