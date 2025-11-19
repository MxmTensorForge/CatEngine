#ifndef COLLIDER_H
#define COLLIDER_H

#include "../Component.h"
#include "../../Mxm/Vec3.h"
#include <vector>
#include <memory>

class Collider final : public Component
{
private:
	std::vector<Mxm::Vec3> _vertices{};
public:
	void generateFromMesh();
	void generateSimpleFromMesh();

	const std::vector<Mxm::Vec3>& getVertices() const noexcept;
};

#endif // !COLLIDER_H
