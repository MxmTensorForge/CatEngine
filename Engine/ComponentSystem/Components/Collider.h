#ifndef COLLIDER_H
#define COLLIDER_H

#include "../Component.h"
#include "../../Mxm/Vec3.h"
#include "../../Geometry/AABB.h"
#include "../../Geometry/MeshData.h"
#include <vector>
#include <memory>

class Collider final : public Component
{
private:
	std::vector<Mxm::Vec3> _vertices{};
	std::shared_ptr<MeshData> _meshData = nullptr;
	bool _useSimple = false;

	AABB _localAABB{};
	AABB _worldAABB{};

	bool _needsRecalc = true;
public:
	void start() override;
	void update() override;

	static AABB calculateAABB(const std::vector<Mxm::Vec3>& verts, float eps) noexcept;
	static bool checkAABB(const Collider& collider1, const Collider& collider2) noexcept;

	void generateLocalAABB() noexcept;
	const AABB& getLocalAABB() const noexcept { return _localAABB; }

	void generateFromMesh();
	void generateSimpleFromMesh();

	const std::vector<Mxm::Vec3>& getVertices() const noexcept;
};

#endif // !COLLIDER_H
