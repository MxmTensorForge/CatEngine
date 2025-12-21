#ifndef RENDERER_H
#define RENDERER_H

#include <memory>

#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"

#include "../Mxm/Mat4.h"
#include "GPUData.h"

class Camera;
class MeshComponent;

class Renderer final
{
private:
	std::unique_ptr<Shader> _shader;
public:
	Renderer();
	~Renderer();

	void init();
	void update(const std::shared_ptr<Camera>& camera);

	void clear(const Mxm::Vec4& color) const noexcept;
	void viewport(GLsizei width, GLsizei height) const noexcept;

	void drawMesh(const Mxm::Mat4& model, const std::shared_ptr<MeshComponent>& mesh);
};

#endif // !RENDERER_H
