#ifndef MESHDATA_H
#define MESHDATA_H

#include <vector>
#include "../Mxm/Vec3.h"
#include "../Graphics/Color.h"

struct MeshData {
	std::vector<Mxm::Vec3> vertices;
	std::vector<Mxm::Vec3> normals;

	void calculateNormals() noexcept;
};

#endif // !MESH_H
