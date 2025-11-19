#include "RigidBody.h"
#include "MeshComponent.h"

#include "../Object/Transform.h"
#include "../Object/GameObject.h"

#include "../../Core/Logger.h"
#include "../../Core/Time.h"
#include "../../Core/EngineConsts.h"

#include <vector>
#include <array>

Mxm::Vec3 RigidBody::furthestPoint(const Mxm::Vec3& dir) {
	auto mesh = getObject()->getComponent<MeshComponent>();
	if (!mesh) {
		Logger::getInstance().log(LogType::Fatal, "Object in support hasn't mesh component!");
	}

	auto& transform = getObject()->transform();
	Mxm::Vec3 localDir = (transform.getWorldMatrix().transposed() * Mxm::Vec4(dir, 1.0f)).toVec3(); //UPD: нам нужно игнорировать смещение, оставлять масштаб и инвертировать поворот

	float greatestDot = -std::numeric_limits<float>::max();
	Mxm::Vec3 furthestPoint{};

	for (const auto p : mesh->getVertices()) {
		float currDot = localDir.dot(p);
		if (greatestDot < currDot) {
			furthestPoint = p;
			greatestDot = currDot;
		}
	}
	return (transform.getWorldMatrix() * Mxm::Vec4(furthestPoint, 1.0f)).toVec3();
}
Mxm::Vec3 RigidBody::minkowskiDifference(const std::shared_ptr<RigidBody>& object, const Mxm::Vec3& dir) {
	return furthestPoint(dir) - object->furthestPoint(-dir);
}

bool RigidBody::handleSimplex(std::deque<Mxm::Vec3>& simplex, Mxm::Vec3& direction) {
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

std::pair<bool, std::deque<Mxm::Vec3>> RigidBody::gjkCollision(const std::shared_ptr<RigidBody>& object) {
	Mxm::Vec3 direction = Mxm::Vec3(1.0f, 0.0f, 0.0f);

	Mxm::Vec3 support = minkowskiDifference(object, direction);

	_simplex.clear();
	_simplex.push_front(support);

	direction = -support;

	int iters = 0;
	while (iters < 32) {
		support = minkowskiDifference(object, direction);

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

std::pair<Triangle, float> RigidBody::findClosestFace(const std::vector<Triangle>& polytope) {
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
void RigidBody::expandPolytope(std::vector<Triangle>& polytope, const Mxm::Vec3& newPoint) {
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
CollisionResult RigidBody::simplifiedEPA(const std::shared_ptr<RigidBody>& object, const std::deque<Mxm::Vec3>& simplex) {
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
		Mxm::Vec3 newPoint = minkowskiDifference(object, closestFace.normal());

		//Если мы достигли границ множества Минковского, то нам нет смысла продолжать алгоритм
		float newDist = closestFace.normal().dot(newPoint);
		if (newDist - closestDist < Mxm::Consts::EPS * 5.0f) {
			return { closestFace.normal(), closestDist};
		}

		expandPolytope(_polytope, newPoint);
	}
	return { Mxm::Vec3(1.0f, 0.0f, 0.0f), 0.1f};
}

void RigidBody::resolveCollision(const CollisionResult& result, const std::shared_ptr<RigidBody>& object1, const std::shared_ptr<RigidBody>& object2) {
	bool obj1Dynamic = object1->getDynamicState();
    bool obj2Dynamic = object2->getDynamicState();
	if (!obj1Dynamic && !obj2Dynamic) return;

	float d1 = obj1Dynamic && obj2Dynamic ? 0.5f : (obj1Dynamic ? 1.0 : 0.0f);
	float d2 = obj1Dynamic && obj2Dynamic ? 0.5f : (obj2Dynamic ? 1.0 : 0.0f);

	auto move = result.normal * result.depth;
    if (obj1Dynamic) {
        object1->getObject()->transform().translate(-move * d1);
    }
    if (obj2Dynamic) {
        object2->getObject()->transform().translate(move * d2);
    }

	auto projectVelocity = [&](const std::shared_ptr<RigidBody>& obj) {
		Mxm::Vec3 vel = obj->getVelocity();
		Mxm::Vec3 normalVel = result.normal * result.normal.dot(vel);

		obj->setVelocity(vel - normalVel);
		obj->_isCollision = true;
	};

	if (obj1Dynamic) projectVelocity(object1);
	if (obj2Dynamic) projectVelocity(object2);

	if (object1->_collisionCallback) object1->_collisionCallback(object2, result);
	if (object2->_collisionCallback) object2->_collisionCallback(object1, result);

	if (result.depth > 0.5f) {
		for (const auto& v : object1->_simplex) {
			Logger::getInstance().log(LogType::Message, std::to_string(v.x) + "   " + std::to_string(v.y) + "   " + std::to_string(v.z));
		}
	}
}

void RigidBody::updatePhysics() {
	if (!_isDynamic) return;

	_velocity += _acceleration * Time::deltaTime();
	getObject()->transform().translate(_velocity * Time::deltaTime());

	float friction = _isCollision ? _friction : _airFriction;
	Mxm::Vec3 frictionForce = -_velocity * friction;
	_velocity += frictionForce * Time::deltaTime();

	_isCollision = false;
}

void RigidBody::setCollisionCallback(const std::function<void(std::shared_ptr<RigidBody>, const CollisionResult&)>& func) {
	_collisionCallback = func;
}