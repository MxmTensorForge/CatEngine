#ifndef TRANSFORMANIM_H
#define TRANSFORMANIM_H

#include "Animation.h"
#include "../../ComponentSystem/Object/GameObject.h"

#include <memory>

namespace Animations
{
	class TranslateByAnim final : public Animation
	{
	private:
		GameObject* _object;
		Mxm::Vec3 _value;

		void update() override {
			if (!_object) { stop(); return; }
			_object->transform().translate(_value * deltaProgress());
		}
	public:
		template <typename... Args>
		TranslateByAnim(GameObject* object, const Mxm::Vec3& value, Args&&... args)
			: Animation(std::forward<Args>(args)...), _object(object), _value(value) {
		}
	};

	class TranslateToAnim final : public Animation
	{
	private:
		GameObject* _object;

		bool _initialized = false;
		Mxm::Vec3 _start;
		Mxm::Vec3 _end;

		void update() override {
			if (!_object) { stop(); return; }

			if (!_initialized) {
				_start = _object->transform().getPosition();
				_initialized = true;
			}

			_object->transform().setPosition(_start + (_end - _start) * progress());
		}
	public:
		template <typename... Args>
		TranslateToAnim(GameObject* object, const Mxm::Vec3& value, Args&&... args)
			: Animation(std::forward<Args>(args)...), _object(object), _end(value) {
		}
	};

	class RotateByAnim final : public Animation
	{
	private:
		GameObject* _object;
		Mxm::Vec3 _value;

		void update() override {
			if (!_object) { stop(); return; }

			_object->transform().rotate(_value * deltaProgress());
		}
	public:
		template <typename... Args>
		RotateByAnim(GameObject* object, const Mxm::Vec3& value, Args&&... args)
			: Animation(std::forward<Args>(args)...), _object(object), _value(value) {
		}
	};

	class RotateToAnim final : public Animation
	{
	private:
		GameObject* _object;

		bool _initialized = false;
		Mxm::Vec3 _start;
		Mxm::Vec3 _end;

		void update() override {
			if (!_object) { stop(); return; }

			if (!_initialized) {
				_start = _object->transform().getRotation();
				_initialized = true;
			}

			_object->transform().setRotation(_start + (_end - _start) * progress());
		}
	public:
		template <typename... Args>
		RotateToAnim(GameObject* object, const Mxm::Vec3& value, Args&&... args)
			: Animation(std::forward<Args>(args)...), _object(object), _end(value) {
		}
	};

	class ScaleToAnim final : public Animation
	{
	private:
		GameObject* _object;

		bool _initialized = false;
		Mxm::Vec3 _start;
		Mxm::Vec3 _end;

		void update() override {
			if (!_object) { stop(); return; }

			if (!_initialized) {
				_start = _object->transform().getScale();
				_initialized = true;
			}

			_object->transform().setScale(_start + (_end - _start) * progress());
		}
	public:
		template <typename... Args>
		ScaleToAnim(GameObject* object, const Mxm::Vec3& value, Args&&... args)
			: Animation(std::forward<Args>(args)...), _object(object), _end(value) {
		}
	};
}

#endif