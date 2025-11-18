#ifndef MESHDATA_H
#define MESHDATA_H

#include <vector>
#include "../Mxm/Vec3.h"
#include "../Graphics/Color.h"

struct MeshData {
	std::vector<Mxm::Vec3> vertices;
	std::vector<unsigned int> indices;
	std::vector<Color> triangleColors;
};

#endif // !MESH_H
