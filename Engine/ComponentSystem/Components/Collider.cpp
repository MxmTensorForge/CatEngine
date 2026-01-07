#include "Collider.h"
#include "../Object//GameObject.h"
#include "MeshComponent.h"

#include <iostream>

void Collider::start() {
	if (_needsRecalc) {
		generateLocalAABB();
	}
}
void Collider::update() {
	if (_needsRecalc) {
		generateLocalAABB();
	}

	const auto& transform = getObject()->transform();
	const auto& worldMatrix = transform.getWorldMatrix();

	_worldAABB.center = (worldMatrix * Mxm::Vec4(_localAABB.center, 1.0f)).toVec3();
	_worldAABB.extent = (worldMatrix.abs() * Mxm::Vec4(_localAABB.extent, 0.0f)).toVec3();

	//std::cout << "Name: " << getObject()->getName() << '\n';
	//std::cout << "Center: X= " << _worldAABB.center.x << " Y=" << _worldAABB.center.y << " Z=" << _worldAABB.center.z << '\n';
	//std::cout << "Extent: X= " << _worldAABB.extent.x << " Y=" << _worldAABB.extent.y << " Z=" << _worldAABB.extent.z << '\n';
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
	auto mesh = getObject()->getComponent<MeshComponent>();
	if (!mesh) return;

	_localAABB = calculateAABB(mesh->getVertices(), 0.1f);
	_needsRecalc = false;
}

void Collider::generateFromMesh() {
	//TODO: a real implementation of dividing a concave figure into several convex ones
	auto mesh = getObject()->getComponent<MeshComponent>();
	if (!mesh) return;

	_meshData = mesh->getData();
	generateLocalAABB();

	_useSimple = false;
}
void Collider::generateSimpleFromMesh() {
	auto mesh = getObject()->getComponent<MeshComponent>();
	if (!mesh) return;

	AABB aabb = calculateAABB(mesh->getVertices(), 0.1f);
	_vertices.clear();
	_vertices.emplace_back(aabb.center + Mxm::Vec3(-aabb.extent.x, -aabb.extent.y, -aabb.extent.z));
	_vertices.emplace_back(aabb.center + Mxm::Vec3(aabb.extent.x, -aabb.extent.y, -aabb.extent.z));
	_vertices.emplace_back(aabb.center + Mxm::Vec3(-aabb.extent.x, aabb.extent.y, -aabb.extent.z));
	_vertices.emplace_back(aabb.center + Mxm::Vec3(-aabb.extent.x, -aabb.extent.y, aabb.extent.z));
	_vertices.emplace_back(aabb.center + Mxm::Vec3(aabb.extent.x, aabb.extent.y, -aabb.extent.z));
	_vertices.emplace_back(aabb.center + Mxm::Vec3(-aabb.extent.x, aabb.extent.y, aabb.extent.z));
	_vertices.emplace_back(aabb.center + Mxm::Vec3(aabb.extent.x, aabb.extent.y, aabb.extent.z));
	_vertices.emplace_back(aabb.center + Mxm::Vec3(aabb.extent.x, -aabb.extent.y, aabb.extent.z));

	_localAABB = aabb;

	_useSimple = true;
}

const std::vector<Mxm::Vec3>& Collider::getVertices() const noexcept {
	return _useSimple ? _vertices : _meshData->vertices;
}