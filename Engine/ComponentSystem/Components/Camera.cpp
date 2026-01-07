#include "Camera.h"
#include "../../Core/EngineConsts.h"

#include "../Object/GameObject.h"
#include "../Object/Transform.h"
#include "MeshComponent.h"

#include <iostream>

void Camera::recalcProjection() noexcept {
	_projectionMatrix = Mxm::Mat4::perspective(_fov, _aspect, _zNear, _zFar);
	//_projectionMatrix = Mxm::Mat4::ortho(10.0f, -10.0f, 10.0f, -10.0f, 0.01f, 300.0f);
}

void Camera::start() {
	_aspect = (float)EngineConsts::STANDART_WIDTH / (float)EngineConsts::STANDART_HEIGHT;
	recalcProjection();
}
void Camera::update() {
	auto& camera_transform = getObject()->transform();
	_viewMatrix = Mxm::Mat4::view(camera_transform.getRight(), camera_transform.getUp(), camera_transform.getForward(), camera_transform.getPosition());
}

void Camera::setFov(float fov) noexcept {
	_fov = fov;
	recalcProjection();
}
void Camera::setNear(float near) noexcept {
	_zNear = near;
	recalcProjection();
}
void Camera::setFar(float far) noexcept {
	_zFar = far;
	recalcProjection();
}