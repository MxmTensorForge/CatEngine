#ifndef CAMERA_H
#define CAMERA_H

#include "../Component.h"

#include "../../Geometry/Triangle.h"
#include "../../Geometry/Plane.h"
#include <vector>

class MeshComponent;
class Transform;

class Camera final : public Component
{
private:
	float _aspect{};
	float _zNear{}, _zFar{};
	float _fov{};

	Mxm::Mat4 _projectionMatrix{};
	Mxm::Mat4 _viewMatrix{};

	void start() override;
	void update() override;
public:
	const Mxm::Mat4& getViewMatrix() const { return _viewMatrix; }
	const Mxm::Mat4& getProjectionMatrix() const { return _projectionMatrix; }
	Mxm::Mat4 getViewProjection() const { return _projectionMatrix * _viewMatrix; }
};

#endif