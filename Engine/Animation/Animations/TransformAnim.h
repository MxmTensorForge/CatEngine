#ifndef TRANSFORMANIM_H
#define TRANSFORMANIM_H

#include "Animation.h"
#include "../../ComponentSystem/Object/Transform.h"

#include <memory>

class TranslateAnim final : public Animation
{
private:
	Transform* _object;
	Mxm::Vec3 _value;

	void update() override {
		if (!_object) { stop(); return; }
		_object->translate(_value * deltaProgress());
	}
public:
	template <typename... Args>
	TranslateAnim(Transform* transform, const Mxm::Vec3& value, Args&&... args)
		: Animation(std::forward<Args>(args)...), _object(transform), _value(value) {
	}
};

class TranslateToPointAnim final : public Animation
{
private:
	Transform* _object;

	bool _initialized = false;
	Mxm::Vec3 _start;
	Mxm::Vec3 _end;

	void update() override {
		if (!_object) { stop(); return; }

		if (!_initialized) {
			_start = _object->getPosition();
			_initialized = true;
		}

		_object->setPosition(_start + (_end - _start) * progress());
	}
public:
	template <typename... Args>
	TranslateToPointAnim(Transform* transform, const Mxm::Vec3& value, Args&&... args)
		: Animation(std::forward<Args>(args)...), _object(transform), _end(value) {
	}
};

class RotateByAnim final : public Animation
{
private:
	Transform* _object;
	Mxm::Vec3 _value;

	void update() override {
		if (!_object) { stop(); return; }

		_object->rotate(_value * deltaProgress());
	}
public:
	template <typename... Args>
	RotateByAnim(Transform* transform, const Mxm::Vec3& value, Args&&... args)
		: Animation(std::forward<Args>(args)...), _object(transform), _value(value) {
	}
};

class RotateToAnim final : public Animation
{
private:
	Transform* _object;

	bool _initialized = false;
	Mxm::Vec3 _start;
	Mxm::Vec3 _end;

	void update() override {
		if (!_object) { stop(); return; }

		if (!_initialized) {
			_start = _object->getRotation();
			_initialized = true;
		}

		_object->setRotation(_start + (_end - _start) * progress());
	}
public:
	template <typename... Args>
	RotateToAnim(Transform* transform, const Mxm::Vec3& value, Args&&... args)
		: Animation(std::forward<Args>(args)...), _object(transform), _end(value) {
	}
};

class SetScaleAnim final : public Animation
{
private:
	Transform* _object;

	bool _initialized = false;
	Mxm::Vec3 _start;
	Mxm::Vec3 _end;

	void update() override {
		if (!_object) { stop(); return; }

		if (!_initialized) {
			_start = _object->getScaling();
			_initialized = true;
		}

		_object->setScaling(_start + (_end - _start) * progress());
	}
public:
	template <typename... Args>
	SetScaleAnim(Transform* transform, const Mxm::Vec3& value, Args&&... args)
		: Animation(std::forward<Args>(args)...), _object(transform), _end(value) {
	}
};

#endif