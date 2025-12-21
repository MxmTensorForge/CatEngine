#include "Camera.h"
#include "../../Core/EngineConsts.h"

#include "../Object/GameObject.h"
#include "../Object/Transform.h"
#include "MeshComponent.h"

#include <iostream>

void Camera::start() {
	_zNear = 0.1f;
	_zFar = 3000.0f;
	_fov = Mxm::Consts::DEG2RAD * 90.0f;
	_aspect = (float)EngineConsts::STANDART_WIDTH / (float)EngineConsts::STANDART_HEIGHT;

	_projectionMatrix = Mxm::Mat4::perspective(_fov, _aspect, _zNear, _zFar);
}
void Camera::update() {
	auto& camera_transform = getObject()->transform();
	_viewMatrix = Mxm::Mat4::view(camera_transform.getRight(), camera_transform.getUp(), camera_transform.getForward(), camera_transform.getPosition());
}