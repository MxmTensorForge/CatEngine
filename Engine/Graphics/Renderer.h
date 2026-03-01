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
class Material;

class PointLight;
class DirectionLight;

class Renderer final
{
private:
	std::unique_ptr<Shader> _shader;
	std::unique_ptr<Shader> _lightShader;
public:
	Renderer();
	~Renderer();

	void init();
	void update(const Camera* camera, 
		const std::vector<PointLight*>& pointLights, const DirectionLight* directionLight);

	void clear(Color color) const noexcept;
	void viewport(GLsizei width, GLsizei height) const noexcept;

	void setDrawFrame(bool state) const noexcept;
	void setAmbientColor(Color color) const noexcept;

	void drawMesh(const Mxm::Mat4& model, const Material* material);
};

#endif // !RENDERER_H