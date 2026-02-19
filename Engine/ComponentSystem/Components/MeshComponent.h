#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "../Component.h"
#include "../../Geometry/MeshData.h"

#include <vector>
#include <string>
#include <memory>

struct MeshData;

struct IntersectionInfo
{
	float distance;
	Mxm::Vec3 point;
	GameObject* gameObject;
	std::string tag;
};

class MeshComponent final : public Component
{
private:
	std::shared_ptr<MeshData> _mesh;
public:
	MeshComponent(const std::string& meshName);

	inline const std::vector<Mxm::Vec3>& getVertices() const noexcept { return _mesh->vertices; }
	inline const std::vector<unsigned int>& getIndices() const noexcept { return _mesh->indices; }

	inline const std::shared_ptr<MeshData>& getData() const noexcept { return _mesh; }

	bool intersection(const Mxm::Vec3& origin, const Mxm::Vec3& dir, IntersectionInfo& out);
};

#endif