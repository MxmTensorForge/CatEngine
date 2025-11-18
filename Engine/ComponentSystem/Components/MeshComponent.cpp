#include "MeshComponent.h"
#include "../../Geometry/Triangle.h"
#include "../../Core/Logger.h"

#include "../../ComponentSystem/Object/GameObject.h"

#include <fstream>
#include <string>
#include <sstream>

MeshComponent::MeshComponent(const std::shared_ptr<MeshData>& mesh, Color color) : _mesh{mesh}, _triangleColors(mesh->triangleColors.size(), color)
{
}
MeshComponent::MeshComponent(const std::shared_ptr<MeshData>& mesh) : _mesh{mesh}, _triangleColors(mesh->triangleColors)
{
}

void MeshComponent::applyColorToAllTriangles(Color color) {
	_triangleColors.assign(_triangleColors.size(), color);
}

bool MeshComponent::intersection(const Mxm::Vec3& origin, const Mxm::Vec3& dir, IntersectionInfo& out) {
	const auto& vertices = getVertices();
	const auto& indices = getIndices();

	auto& model = getObject()->transform().getWorldMatrix();
	Mxm::Mat4 inverseModel = getObject()->transform().getInverseWorldMatrix();

	Mxm::Vec3 invOrigin = (inverseModel * Mxm::Vec4(origin, 1.0f)).toVec3();
	Mxm::Vec3 invDir = (inverseModel * Mxm::Vec4(dir, 0.0f)).toVec3();

	Mxm::Vec3 intersection_point;
	float dist{};

	float closest_distance = std::numeric_limits<float>::max();
	bool hit = false;
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		size_t idx0 = indices[i];
		size_t idx1 = indices[i + 1];
		size_t idx2 = indices[i + 2];

		Triangle tri = Triangle(vertices[idx0], vertices[idx1], vertices[idx2]);
		//if ((origin - Vector3D(tri[0])).dot(tri.normal()) < 0.0f) continue;

		if (tri.intersection(invOrigin, invDir, intersection_point, dist)) {
			if (dist < closest_distance) {
				closest_distance = dist;

				auto obj = getObject();
				out = { dist, (model * Mxm::Vec4(intersection_point, 1.0f)).toVec3(), obj, obj->getTag()};
				hit = true;
			}
		}
	}
	return hit;
}