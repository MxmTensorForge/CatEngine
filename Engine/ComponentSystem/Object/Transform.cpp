#include "Transform.h"

void Transform::translate(const Mxm::Vec3& vec) noexcept {
	_position += vec;
	markDirty();
}
void Transform::rotate(const Mxm::Vec3& vec) noexcept {
	_rotation += vec;
	markDirty();
}
void Transform::scale(const Mxm::Vec3& vec) noexcept {
	_scaling *= vec;
	markDirty();
}

void Transform::setPosition(const Mxm::Vec3& vec) noexcept {
	_position = vec;
	markDirty();
}
void Transform::setRotation(const Mxm::Vec3& vec) noexcept {
	_rotation = vec;
	markDirty();
}
void Transform::setScaling(const Mxm::Vec3& vec) noexcept {
	_scaling = vec;
	markDirty();
}

void Transform::setLookRotation(const Mxm::Vec3& direction) noexcept {
	Mxm::Vec3 targetForward = direction.normalized();

	float yaw = atan2f(targetForward.x, targetForward.z);
	float pitch = -asinf(targetForward.y);

	_rotation = Mxm::Vec3(pitch, yaw, 0);
	markDirty();
}
void Transform::setLookAt(const Mxm::Vec3& target) noexcept {
	Mxm::Vec3 direction = (target - _position).normalized();
	setLookRotation(direction);
}

const Mxm::Vec3 Transform::getRight() const noexcept {
	getWorldMatrix();
	return _world.col(0).toVec3().normalized();
}
const Mxm::Vec3 Transform::getUp() const noexcept {
	getWorldMatrix();
	return _world.col(1).toVec3().normalized();
}
const Mxm::Vec3 Transform::getForward() const noexcept {
	getWorldMatrix();
	return _world.col(2).toVec3().normalized();
}

Mxm::Vec3 Transform::getWorldPosition() const noexcept {
	getWorldMatrix();
	return _world.col(3).toVec3();
}
Mxm::Vec3 Transform::getWorldRotation() const noexcept {
	const Mxm::Mat4& world = getWorldMatrix();

	float pitch = -asinf(world.col(2).y);
	float yaw = atan2f(world.col(2).x, world.col(2).z);
	float roll = atan2f(world.col(1).x, world.col(0).x);

	return Mxm::Vec3(pitch, yaw, roll);
}
Mxm::Vec3 Transform::getWorldScaling() const noexcept {
	const Mxm::Mat4& world = getWorldMatrix();

	float scaleX = world.col(0).toVec3().length();
	float scaleY = world.col(1).toVec3().length();
	float scaleZ = world.col(2).toVec3().length();

	return Mxm::Vec3(scaleX, scaleY, scaleZ);
}

bool Transform::isChildOf(Transform* potentialParent) const noexcept {
	if (!potentialParent) return false;;

	const Transform* current = this;
	while (current) {
		if (current == potentialParent) return true;
		current = current->getParent();
	}
	return false;
}

void Transform::setParent(Transform* newParent) noexcept {
	auto oldParent = _parent;
	if (oldParent == newParent || this == newParent) return;

	if (newParent->isChildOf(this) && newParent) {
		return;
	}

	if (oldParent) oldParent->removeChild(this);
	_parent = newParent;

	if (newParent) newParent->_children.push_back(this);
	markDirty();
}

void Transform::addChild(Transform* child) noexcept {
	if (!child || child == this) return;

	child->setParent(this);
}
void Transform::removeChild(Transform* child) noexcept {
	if (!child) return;
	_children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
}

const Mxm::Mat4& Transform::getModelMatrix() const noexcept {
	if (_isDirty) {
		_model = Mxm::Mat4::translation(_position) * Mxm::Mat4::rotationY(_rotation.y) * Mxm::Mat4::rotationX(_rotation.x) * Mxm::Mat4::rotationZ(_rotation.z) * Mxm::Mat4::scaling(_scaling);
	}
	return _model;
}
const Mxm::Mat4& Transform::getWorldMatrix() const noexcept {
	if (_isDirty) {
		getModelMatrix();
		if (_parent) {
			_world = _parent->getWorldMatrix() * _model;
		}
		else {
			_world = _model;
		}
		_isDirty = false;
	}
	return _world;
}
Mxm::Mat4 Transform::getWorldMatrixWithoutScale() const noexcept {
	Mxm::Mat4 model = Mxm::Mat4::translation(_position) *
		Mxm::Mat4::rotationY(_rotation.y) * Mxm::Mat4::rotationX(_rotation.x) * Mxm::Mat4::rotationZ(_rotation.z);

	if (_parent) {
		return _parent->getWorldMatrixWithoutScale() * model;
	}
	return model;
}

Mxm::Mat4 Transform::getInverseModelMatrix() const noexcept {
	Mxm::Mat4 inverse;

	Mxm::Vec3 invScale = Mxm::Vec3(1.0f / _scaling.x, 1.0f / _scaling.y, 1.0f / _scaling.z);
	inverse = Mxm::Mat4::scaling(invScale) * (Mxm::Mat4::rotationY(_rotation.y) * Mxm::Mat4::rotationX(_rotation.x) * Mxm::Mat4::rotationZ(_rotation.z)).transposed() * Mxm::Mat4::translation(-_position);

	return inverse;
}
Mxm::Mat4 Transform::getInverseWorldMatrix() const noexcept {
	Mxm::Mat4 myInv = getInverseModelMatrix();
	if (_parent) {
		return myInv * _parent->getInverseWorldMatrix();
	}
	return myInv;
}

void Transform::markDirty() noexcept {
	_isDirty = true;
	for (auto& child : _children) {
		if (child) {
			child->markDirty();
		}
	}
}