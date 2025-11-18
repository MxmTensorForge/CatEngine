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

	std::vector<Plane> _clippingPlanes;

	std::vector<Triangle> _clippedTris;
	std::vector<Triangle> _tempClippedTris;

	std::vector<Mxm::Vec4> _outsidePoints;
	std::vector<Mxm::Vec4> _insidePoints;

	std::vector<Triangle> _projectedTris;

	Mxm::Mat4 _projectionMatrix{};
	Mxm::Mat4 _viewMatrix{};
	Mxm::Mat4 _screenSpaceMatrix{};

	void start() override;
	void update() override;
public:
	const std::vector<Triangle>& project(const std::shared_ptr<MeshComponent>& mesh, const Transform& transform);
};

#endif