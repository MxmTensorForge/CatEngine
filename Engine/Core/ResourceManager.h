#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <unordered_map>
#include <string>
#include <memory>

#include "../Geometry/MeshData.h"

class ResourceManager final
{
private:
	std::unordered_map<std::string, std::shared_ptr<MeshData>> _meshes;

	ResourceManager() = default;
	~ResourceManager() = default;
public:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	static ResourceManager& getInstance();

	void loadModelFromFile(const std::string& name, const std::string& path);
	const std::shared_ptr<MeshData>& getModel(const std::string& name) const;

	void removeModel(const std::string& name);
	void clearModels(const std::string& name);
};

#endif // !RESOURCEMANAGER_H
