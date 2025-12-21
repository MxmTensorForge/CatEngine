#include "Renderer.h"

#include "../ComponentSystem/Components/MeshComponent.h"
#include "../ComponentSystem/Components/Camera.h"

#include "../ComponentSystem/Object/GameObject.h"

Renderer::Renderer() : _shader(nullptr) {}
Renderer::~Renderer() {}

void Renderer::init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_shader = std::make_unique<Shader>("shaders/shader.vert", "shaders/shader.frag");
	_shader->use();
}
void Renderer::update(const std::shared_ptr<Camera>& camera) {
	_shader->setUniform("uProjection", camera->getProjectionMatrix().data(), true);
	_shader->setUniform("uView", camera->getViewMatrix().data(), true);
}

void Renderer::clear(const Mxm::Vec4& color) const noexcept {
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Renderer::viewport(GLsizei width, GLsizei height) const noexcept {
	glViewport(0, 0, width, height);
}

void Renderer::drawMesh(const Mxm::Mat4& model, const std::shared_ptr<MeshComponent>& mesh) {
	_shader->use();
	_shader->setUniform("uModel", model.data(), true);

	Color color = mesh->getColor();
	_shader->setUniform("uColor", color.rf(), color.gf(), color.bf(), color.af());

	if (color.af() < 1.0f) {
		glDepthMask(GL_FALSE);
	}
	mesh->getGPUData().draw();
	glDepthMask(GL_TRUE);
}