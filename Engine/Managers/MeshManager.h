#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include <unordered_map>
#include <string>
#include <memory>

struct MeshData;

class MeshManager final
{
private:
	std::unordered_map<std::string, std::shared_ptr<MeshData>> _meshes;

	MeshManager() = default;
	~MeshManager() = default;
public:
	MeshManager(const MeshManager&) = delete;
	MeshManager& operator=(const MeshManager&) = delete;
	MeshManager(MeshManager&&) = delete;
	MeshManager& operator=(MeshManager&&) = delete;

	static MeshManager& getInstance();

	void loadModelFromFile(const std::string& name, const std::string& path);
	const std::shared_ptr<MeshData>& getModel(const std::string& name) const;

	void removeModel(const std::string& name);
	void clearModels(const std::string& name);
};

#endif // !RESOURCEMANAGER_H
