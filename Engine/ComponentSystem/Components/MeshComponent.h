#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "../Component.h"
#include "../../Geometry/MeshData.h"

#include <vector>
#include <string>
#include <memory>

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
	std::vector<Color> _triangleColors;
public:
	MeshComponent(const std::shared_ptr<MeshData>& mesh, Color color);
	MeshComponent(const std::shared_ptr<MeshData>& mesh);
	MeshComponent() = default;

	inline std::vector<Mxm::Vec3>& getVertices() noexcept { return _mesh->vertices; }
	inline std::vector<unsigned int>& getIndices() noexcept { return _mesh->indices; }

	inline Color getTriangleColor(size_t idx) const noexcept {
		if (idx >= _triangleColors.size()) return Color(255, 255, 255);
		return _triangleColors[idx];
	}
	void applyColorToAllTriangles(Color color);

	bool intersection(const Mxm::Vec3& origin, const Mxm::Vec3& dir, IntersectionInfo& out);
};

#endif