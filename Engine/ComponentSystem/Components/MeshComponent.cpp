#include "MeshComponent.h"
#include "../../Geometry/Triangle.h"
#include "../../Core/Logger.h"

#include "../../ComponentSystem/Object/GameObject.h"

#include <fstream>
#include <string>
#include <sstream>

MeshComponent::MeshComponent(const std::shared_ptr<MeshData>& mesh, Color color) : _mesh{mesh}, _color(color), _data()
{
	_data.loadData(_mesh->vertices, _mesh->normals, _mesh->textureCoords);
}
MeshComponent::MeshComponent(const std::shared_ptr<MeshData>& mesh, const std::string& textureName) : _mesh{ mesh }, _textureName(textureName), _data()
{
	_data.loadData(_mesh->vertices, _mesh->normals, _mesh->textureCoords);
}
MeshComponent::MeshComponent(const std::shared_ptr<MeshData>& mesh) : _mesh{mesh}, _color(Color(255, 255, 255, 255)), _data()
{
	_data.loadData(_mesh->vertices, _mesh->normals, _mesh->textureCoords);
}

bool MeshComponent::intersection(const Mxm::Vec3& origin, const Mxm::Vec3& dir, IntersectionInfo& out) {
	const auto& vertices = getVertices();

	auto& model = getObject()->transform().getWorldMatrix();
	Mxm::Mat4 inverseModel = getObject()->transform().getInverseWorldMatrix();

	Mxm::Vec3 invOrigin = (inverseModel * Mxm::Vec4(origin, 1.0f)).toVec3();
	Mxm::Vec3 invDir = (inverseModel * Mxm::Vec4(dir, 0.0f)).toVec3();

	Mxm::Vec3 intersection_point;
	float dist{};

	float closest_distance = std::numeric_limits<float>::max();
	bool hit = false;
	for (size_t i = 0; i < vertices.size(); i += 3)
	{
		Triangle tri = Triangle(vertices[i], vertices[i + 1], vertices[i + 2]);
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