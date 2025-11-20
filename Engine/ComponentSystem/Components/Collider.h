#ifndef COLLIDER_H
#define COLLIDER_H

#include "../Component.h"
#include "../../Mxm/Vec3.h"
#include <vector>
#include <memory>

struct AABB
{
	Mxm::Vec3 center;
	Mxm::Vec3 extent;
};

class Collider final : public Component
{
private:
	std::vector<Mxm::Vec3> _vertices{};

	AABB _localAABB{};
	AABB _worldAABB{};

	bool _needsRecalc = true;
public:
	void start() override;
	void update() override;

	static AABB calculateAABB(const std::vector<Mxm::Vec3>& verts, float eps) noexcept;
	static bool checkAABB(const Collider& collider1, const Collider& collider2) noexcept;

	void generateLocalAABB() noexcept;

	void generateFromMesh();
	void generateSimpleFromMesh();

	const std::vector<Mxm::Vec3>& getVertices() const noexcept;
};

#endif // !COLLIDER_H
