#include "Collider.h"
#include "../Object//GameObject.h"
#include "MeshComponent.h"

void Collider::generateFromMesh() {
	//TODO: a real implementation of dividing a concave figure into several convex ones
	auto mesh = getObject()->getComponent<MeshComponent>();
	if (!mesh) return;

	for (const auto& v : mesh->getVertices()) {
		_vertices.push_back(v);
	}
}
void Collider::generateSimpleFromMesh() {
	auto mesh = getObject()->getComponent<MeshComponent>();
	if (!mesh) return;

	float minX{}, minY{}, minZ{};
	float maxX{}, maxY{}, maxZ{};

	for (const auto& v : mesh->getVertices()) {
		minX = fminf(minX, v.x);
		minY = fminf(minY, v.y);
		minZ = fminf(minZ, v.z);

		maxX = fmaxf(maxX, v.x);
		maxY = fmaxf(maxY, v.y);
		maxZ = fmaxf(maxZ, v.z);
	}

	_vertices.emplace_back(Mxm::Vec3(minX, minY, minZ));
	_vertices.emplace_back(Mxm::Vec3(maxX, minY, minZ));
	_vertices.emplace_back(Mxm::Vec3(minX, maxY, minZ));
	_vertices.emplace_back(Mxm::Vec3(minX, minY, maxZ));
	_vertices.emplace_back(Mxm::Vec3(maxX, maxY, minZ));
	_vertices.emplace_back(Mxm::Vec3(minX, maxY, maxZ));
	_vertices.emplace_back(Mxm::Vec3(maxX, maxY, maxZ));
	_vertices.emplace_back(Mxm::Vec3(maxX, minY, maxZ));
}

const std::vector<Mxm::Vec3>& Collider::getVertices() const noexcept {
	return _vertices;
}