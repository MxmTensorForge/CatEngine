#ifndef COLORANIM_H
#define COLORANIM_H

#include "Animation.h"
#include "../../ComponentSystem/Components/MeshComponent.h"

#include <memory>

class ColorAnim final : public Animation
{
private:
	MeshComponent* _object;

	bool _initialized = false;
	Color _start;
	Color _end;

	void update() override {
		if (!_object) { stop(); return; }

		if (!_initialized) {
			_start = _object->getColor();
			_initialized = true;
		}

		float t = progress();
		Color current(
			static_cast<uint8_t>(_start.r() * (1 - t) + _end.r() * t),
			static_cast<uint8_t>(_start.g() * (1 - t) + _end.g() * t),
			static_cast<uint8_t>(_start.b() * (1 - t) + _end.b() * t),
			static_cast<uint8_t>(_start.a() * (1 - t) + _end.a() * t)
		);
		_object->getColor() = current;
	}
public:
	template <typename... Args>
	ColorAnim(MeshComponent* mesh, Color value, Args&&... args)
		: Animation(std::forward<Args>(args)...), _object(mesh), _end(value) {
	}
};

#endif