#include "MeshData.h"

void MeshData::calculateNormals() noexcept {
	normals.resize(vertices.size());
	for (int i = 0; i < vertices.size(); i += 3)
	{
		const Mxm::Vec3& v0 = vertices[i];
		const Mxm::Vec3& v1 = vertices[i + 1];
		const Mxm::Vec3& v2 = vertices[i + 2];

		Mxm::Vec3 edge0 = v1 - v0;
		Mxm::Vec3 edge1 = v2 - v0;

		Mxm::Vec3 faceNormal = edge0.cross(edge1);
		normals[i / 3] = faceNormal;
	}

	for (auto& n : normals) {
		n = n.normalized();
	}
}