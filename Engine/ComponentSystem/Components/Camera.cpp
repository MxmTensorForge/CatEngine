#include "Camera.h"
#include "../../Core/EngineConsts.h"

#include "../Object/GameObject.h"
#include "../Object/Transform.h"
#include "MeshComponent.h"

#include <iostream>

void Camera::start() {
	_zNear = 0.1f;
	_zFar = 200.0f;
	_fov = Mxm::Consts::DEG2RAD * 90.0f;
	_aspect = (float)EngineConsts::STANDART_WIDTH / (float)EngineConsts::STANDART_HEIGHT;

	_projectionMatrix = Mxm::Mat4::perspective(_fov, _aspect, _zNear, _zFar);
	_screenSpaceMatrix = Mxm::Mat4::screenSpace(EngineConsts::SCALED_WIDTH, EngineConsts::SCALED_HEIGHT);

	_clippingPlanes.reserve(6);

	_clippingPlanes.emplace_back(Plane(Mxm::Vec3(0.0f, 0.0f, 1.0f), Mxm::Vec3(0.0f, 0.0f, _zNear)));
	_clippingPlanes.emplace_back(Plane(Mxm::Vec3(0.0f, 0.0f, -1.0f), Mxm::Vec3(0.0f, 0.0f, _zFar)));

	float theta = _fov * 0.5f;
	float thetaX = atanf(tanf(theta) * _aspect);

	_clippingPlanes.emplace_back(Plane(Mxm::Vec3(cosf(thetaX), 0.0f, sinf(thetaX)), Mxm::Vec3(0.0f,0.0f,0.0f))); // left
	_clippingPlanes.emplace_back(Plane(Mxm::Vec3(-cosf(thetaX), 0.0f, sinf(thetaX)), Mxm::Vec3(0.0f,0.0f,0.0f))); // right

	_clippingPlanes.emplace_back(Plane(Mxm::Vec3(0.0f, -cosf(theta), sinf(theta)), Mxm::Vec3(0.0f, 0.0f, 0.0f))); // top
	_clippingPlanes.emplace_back(Plane(Mxm::Vec3(0.0f, cosf(theta), sinf(theta)), Mxm::Vec3(0.0f, 0.0f, 0.0f))); // bottom
}
void Camera::update() {
	auto& camera_transform = getObject()->transform();
	_viewMatrix = Mxm::Mat4::view(camera_transform.getRight(), camera_transform.getUp(), camera_transform.getForward(), camera_transform.getPosition());
}

const std::vector<Triangle>& Camera::project(const std::shared_ptr<MeshComponent>& mesh, const Transform& transform) {
	const auto& vertices = mesh->getVertices();
	const auto& indices = mesh->getIndices();

	_projectedTris.clear();
	if (vertices.empty() || indices.empty()) return _projectedTris;
	auto& camera_transform = getObject()->transform();

	_projectedTris.reserve(indices.size() / 3);

	Mxm::Mat4 model = transform.getWorldMatrix();

	for (size_t i = 0; i < indices.size(); i += 3)
	{
		int idx0 = indices[i];
		int idx1 = indices[i + 1];
		int idx2 = indices[i + 2];

		Triangle tri =
		{
			vertices[idx0],
			vertices[idx1],
			vertices[idx2],
		};
		tri.applyMatrix(model, true);

		float dot = tri.normal().dot((camera_transform.getWorldPosition() - tri[0].toVec3()).normalized()); //to_camera dot
		if (dot < 0) continue;

		dot = tri.normal().dot((Mxm::Vec3(15.0f, 10.0f, 15.0f) - tri[0].toVec3()).normalized()); //to_light dot

		tri.applyMatrix(_viewMatrix, false);

		float ambient = 0.07f;

		dot = fmaxf(ambient, fminf(dot, 1.0f));
		dot = powf(dot, 0.4f);

		Color triangleColor = mesh->getTriangleColor(i / 3);
		Color color(triangleColor.r() * dot, triangleColor.g() * dot, triangleColor.b() * dot, triangleColor.a());

		_clippedTris.clear();
		_tempClippedTris.clear();
		
		_clippedTris.push_back(tri);
		for (const auto& plane : _clippingPlanes) {
			_tempClippedTris.clear();

			for (const auto& triangle : _clippedTris) {
				plane.clip(triangle, _tempClippedTris, _insidePoints, _outsidePoints);
			}

			_clippedTris.swap(_tempClippedTris);
		}

		for (const auto& t : _clippedTris)
		{
			Triangle clipProjectedTriangle = t;
			clipProjectedTriangle.applyMatrix(_projectionMatrix, false);

			//optimization
			float invW0 = 1.0f / clipProjectedTriangle[0].w;
			float invW1 = 1.0f / clipProjectedTriangle[1].w;
			float invW2 = 1.0f / clipProjectedTriangle[2].w;

			Triangle clipProjectedNormTriangle = Triangle(clipProjectedTriangle[0].toVec3() * invW0,
				clipProjectedTriangle[1].toVec3() * invW1,
				clipProjectedTriangle[2].toVec3() * invW2,
				color);
			clipProjectedNormTriangle.applyMatrix(_screenSpaceMatrix, false);

			_projectedTris.emplace_back(clipProjectedNormTriangle);
		}
	}
	return _projectedTris;
}