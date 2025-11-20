#ifndef TRANSFORMANIM_H
#define TRANSFORMANIM_H

#include "Animation.h"
#include "../../ComponentSystem/Object/GameObject.h"

#include <memory>

class TranslateByAnim final : public Animation
{
private:
	std::weak_ptr<GameObject> _object;
	Mxm::Vec3 _value;

	void update() override {
		auto obj = _object.lock();
		if (!obj) { stop(); return; }
		obj->transform().translate(_value * deltaProgress());
	}
public:
	template <typename... Args>
	TranslateByAnim(const std::shared_ptr<GameObject>& object, const Mxm::Vec3& value, Args&&... args)
		: Animation(std::forward<Args>(args)...), _object(object), _value(value) {
	}
};

class TranslateToAnim final : public Animation
{
private:
	std::weak_ptr<GameObject> _object;

	bool _initialized = false;
	Mxm::Vec3 _start;
	Mxm::Vec3 _end;

	void update() override {
		auto obj = _object.lock();
		if (!obj) { stop(); return; }

		if (!_initialized) {
			_start = obj->transform().getPosition();
			_initialized = true;
		}

		obj->transform().setPosition(_start + (_end - _start) * progress());
	}
public:
	template <typename... Args>
	TranslateToAnim(const std::shared_ptr<GameObject>& object, const Mxm::Vec3& value, Args&&... args)
		: Animation(std::forward<Args>(args)...), _object(object), _end(value) {
	}
};

class RotateByAnim final : public Animation
{
private:
	std::weak_ptr<GameObject> _object;
	Mxm::Vec3 _value;

	void update() override {
		auto obj = _object.lock();
		if (!obj) { stop(); return; }

		obj->transform().rotate(_value * deltaProgress());
	}
public:
	template <typename... Args>
	RotateByAnim(const std::shared_ptr<GameObject>& object, const Mxm::Vec3& value, Args&&... args)
		: Animation(std::forward<Args>(args)...), _object(object), _value(value) {
	}
};

class RotateToAnim final : public Animation
{
private:
	std::weak_ptr<GameObject> _object;

	bool _initialized = false;
	Mxm::Vec3 _start;
	Mxm::Vec3 _end;

	void update() override {
		auto obj = _object.lock();
		if (!obj) { stop(); return; }

		if (!_initialized) {
			_start = obj->transform().getRotation();
			_initialized = true;
		}

		obj->transform().setRotation(_start + (_end - _start) * progress());
	}
public:
	template <typename... Args>
	RotateToAnim(const std::shared_ptr<GameObject>& object, const Mxm::Vec3& value, Args&&... args)
		: Animation(std::forward<Args>(args)...), _object(object), _end(value) {
	}
};

class SetScaleAnim final : public Animation
{
private:
	std::weak_ptr<GameObject> _object;

	bool _initialized = false;
	Mxm::Vec3 _start;
	Mxm::Vec3 _end;

	void update() override {
		auto obj = _object.lock();
		if (!obj) { stop(); return; }

		if (!_initialized) {
			_start = obj->transform().getScale();
			_initialized = true;
		}

		obj->transform().setScale(_start + (_end - _start) * progress());
	}
public:
	template <typename... Args>
	SetScaleAnim(const std::shared_ptr<GameObject>& object, const Mxm::Vec3& value, Args&&... args)
		: Animation(std::forward<Args>(args)...), _object(object), _end(value) {
	}
};

#endif