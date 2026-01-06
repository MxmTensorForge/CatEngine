#ifndef RENDERER_H
#define RENDERER_H

#include <memory>

#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"

#include "../Mxm/Mat4.h"
#include "GPUData.h"

#include <vector>

class Camera;
class MeshComponent;

class PointLight;
class DirectionLight;

class Renderer final
{
private:
	std::unique_ptr<Shader> _shader;
public:
	Renderer();
	~Renderer();

	void init();
	void update(const Camera* camera, 
		const std::vector<PointLight*>& pointLights, const DirectionLight* directionLight);

	void clear(const Mxm::Vec4& color) const noexcept;
	void viewport(GLsizei width, GLsizei height) const noexcept;
	void setDrawFrame(bool state) const noexcept;

	void drawMesh(const Mxm::Mat4& model, const MeshComponent* mesh);
};

#endif // !RENDERER_H