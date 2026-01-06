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
	float distance = 0.0f;
	Mxm::Vec3 point;
	std::shared_ptr<GameObject> gameObject;
	std::string tag;
};

class MeshComponent final : public Component
{
private:
	std::shared_ptr<MeshData> _mesh;
	std::string _textureName;
	Color _color;
public:
	MeshComponent(const std::string& meshName, Color color);
	MeshComponent(const std::string& meshName, const std::string& textureName);
	MeshComponent(const std::string& meshName);
	MeshComponent() = delete;	

	inline const std::vector<Mxm::Vec3>& getVertices() const noexcept { return _mesh->vertices; }
	inline const std::vector<unsigned int>& getIndices() const noexcept { return _mesh->indices; }
	inline const std::string& getTextureName() const noexcept { return _textureName; }

	inline Color& getColor() noexcept { return _color; }
	inline const Color& getColor() const noexcept { return _color; }

	inline const std::shared_ptr<MeshData>& getData() const noexcept { return _mesh; }

	bool intersection(const Mxm::Vec3& origin, const Mxm::Vec3& dir, IntersectionInfo& out);
};

#endif