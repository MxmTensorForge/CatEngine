#include "Collider.h"
#include "../Object//GameObject.h"
#include "MeshComponent.h"

void Collider::start() {
	if (_needsRecalcAABB) {
		generateLocalAABB();
	}
}
void Collider::update() {
	if (_needsRecalcAABB) {
		generateLocalAABB();
	}

	const auto& transform = getObject()->transform();
	const auto& worldMatrix = transform.getWorldMatrix();

	_worldAABB.center = (worldMatrix * Mxm::Vec4(_localAABB.center, 1.0f)).toVec3();
	_worldAABB.extent = (worldMatrix.abs() * Mxm::Vec4(_localAABB.extent, 0.0f)).toVec3();
}

AABB Collider::calculateAABB(const std::vector<Mxm::Vec3>& vertices, float eps) noexcept {
	Mxm::Vec3 min = Mxm::Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	Mxm::Vec3 max = Mxm::Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (const auto& v : vertices) {
		min.x = fminf(min.x, v.x);
		min.y = fminf(min.y, v.y);
		min.z = fminf(min.z, v.z);

		max.x = fmaxf(max.x, v.x);
		max.y = fmaxf(max.y, v.y);
		max.z = fmaxf(max.z, v.z);
	}
	AABB aabb;
	aabb.center = (min + max) * 0.5f;
	aabb.extent = (max - min) * 0.5f + eps;

	return aabb;
}

bool Collider::checkAABB(const Collider& collider1, const Collider& collider2) noexcept {
	AABB aabb1 = collider1._worldAABB;
	AABB aabb2 = collider2._worldAABB;

	Mxm::Vec3 diff = (aabb1.center - aabb2.center).abs();
	Mxm::Vec3 maxDist = aabb1.extent + aabb2.extent;

	return (diff.x <= maxDist.x && diff.y <= maxDist.y && diff.z <= maxDist.z);
}

void Collider::generateLocalAABB() noexcept {
	if (!_shape) return;

	const Mxm::Vec3 directions[] =
	{
		Mxm::Vec3(1.0f, 0.0f, 0.0f), Mxm::Vec3(-1.0f, 0.0f, 0.0f),
		Mxm::Vec3(0.0f, 1.0f, 0.0f), Mxm::Vec3(0.0f, -1.0f, 0.0f),
		Mxm::Vec3(0.0f, 0.0f, 1.0f), Mxm::Vec3(0.0f, 0.0f, -1.0f)
	};

	Mxm::Vec3 min = Mxm::Vec3(FLT_MAX), max = Mxm::Vec3(-FLT_MAX);

	for (const auto& dir : directions) {
		Mxm::Vec3 vert = _shape->support(dir);

		min.x = fminf(min.x, vert.x);
		min.y = fminf(min.y, vert.y);
		min.z = fminf(min.z, vert.z);

		max.x = fmaxf(max.x, vert.x);
		max.y = fmaxf(max.y, vert.y);
		max.z = fmaxf(max.z, vert.z);
	}

	_localAABB.center = (min + max) * 0.5f;
	_localAABB.extent = (max - min) * 0.5f + 0.3f;

	_needsRecalcAABB = false;
}

void Collider::generateFromMesh() {
	auto mesh = getObject()->getComponent<MeshComponent>();
	if (!mesh) return;

	setColliderShape<ConvexHullShape>(mesh->getData().get());
}
void Collider::generateBoxFromMesh() {
	auto mesh = getObject()->getComponent<MeshComponent>();
	if (!mesh) return;

	AABB aabb = calculateAABB(mesh->getVertices(), 0.0f);
	setColliderShape<BoxShape>(aabb.center, aabb.extent);
}

Mxm::Vec3 Collider::support(const Mxm::Vec3& direction) const noexcept {
	if (!_shape) return Mxm::Vec3();
	return _shape->support(direction);
}