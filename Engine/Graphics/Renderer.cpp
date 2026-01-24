#include "Renderer.h"

#include "../ComponentSystem/Components/MeshComponent.h"
#include "../ComponentSystem/Components/Camera.h"

#include "../ComponentSystem/Components/PointLight.h"
#include "../ComponentSystem/Components/DirectionLight.h"

#include "../ComponentSystem/Object/GameObject.h"

#include "../Core/TextureManager.h"
#include "../Core/Logger.h"

#include "../Graphics/Texture.h"

Renderer::Renderer() : _shader(nullptr) {}
Renderer::~Renderer() {}

void Renderer::init() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	_shader = std::make_unique<Shader>("shaders/shader.vert", "shaders/shader.frag");
	_shader->use();

	Logger::getInstance().log(LogType::Message, "Renderer has been successfully initialized");
}
void Renderer::update(const Camera* camera,
	const std::vector<PointLight*>& pointLights, const DirectionLight* directionLight) {

	_shader->use();
	_shader->setUniform("uProjection", camera->getProjectionMatrix().data(), true);
	_shader->setUniform("uView", camera->getViewMatrix().data(), true);

	Mxm::Vec3 camPos = camera->getObject()->transform().getWorldPosition();
	_shader->setUniform("uCameraPos", camPos.x, camPos.y, camPos.z);

	int lightCount = pointLights.size();
	_shader->setUniform("pointLightsCount", lightCount);
	for (int i = 0; i < lightCount; i++) {
		if (i > 8) break;

		auto light = pointLights[i];
		if (!light || !light->getObject()->getActive()) continue;

		std::string name = "pointLights[" + std::to_string(i) + "].";
		_shader->setUniform(name + "linearFading", light->getLinearFading());
		_shader->setUniform(name + "quadraticFading", light->getQuadraticFading());
		_shader->setUniform(name + "intensity", light->getIntensity());

		Color color = light->getLightColor();
		_shader->setUniform(name + "lightColor", color.rf(), color.gf(), color.bf());

		Mxm::Vec3 pos = light->getObject()->transform().getWorldPosition();
		_shader->setUniform(name + "lightPos", pos.x, pos.y, pos.z);
	}

	if (directionLight && directionLight->getObject()->getActive()) {
		std::string name = "directionLight.";
		_shader->setUniform(name + "intensity", directionLight->getIntensity());

		Color color = directionLight->getLightColor();
		_shader->setUniform(name + "lightColor", color.rf(), color.gf(), color.bf());

		Mxm::Vec3 dir = directionLight->getObject()->transform().getForward();
		_shader->setUniform(name + "direction", dir.x, dir.y, dir.z);
	}
}

void Renderer::clear(Color color) const noexcept {
	glClearColor(color.rf(), color.gf(), color.bf(), 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Renderer::viewport(GLsizei width, GLsizei height) const noexcept {
	glViewport(0, 0, width, height);
}
void Renderer::setDrawFrame(bool state) const noexcept {
	glPolygonMode(GL_FRONT_AND_BACK, state ? GL_LINE : GL_FILL);
}
void Renderer::setAmbientColor(Color color) const noexcept {
	_shader->setUniform("uAmbient", color.rf(), color.gf(), color.bf());
}

void Renderer::drawMesh(const Mxm::Mat4& model, const MeshComponent* mesh) {
	_shader->setUniform("uModel", model.data(), true);

	Color color = mesh->getColor();
	_shader->setUniform("uColor", color.rf(), color.gf(), color.bf(), color.af());
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	if (color.a() < 240) {
		glDepthMask(GL_FALSE);
	}

	std::string texName = mesh->getTextureName();
	if (texName.empty()) {
		_shader->setUniform("uUseTexture", 0);
	}
	else {
		TextureManager::getInstance().getTexture(texName)->bind(GL_TEXTURE0);
		_shader->setUniform("uUseTexture", 1);
		_shader->setUniform("uTexture", 0);
	}

	mesh->getData()->data.draw();
	glDepthMask(GL_TRUE);
}