#ifndef SETCOLORANIM_H
#define SETCOLORANIM_H

#include "Animation.h"
#include "../../ComponentSystem/Components/MeshComponent.h"

#include <memory>

class SetColorAnim final : public Animation
{
private:
	std::weak_ptr<MeshComponent> _object;

	bool _initialized = false;
	Color _start;
	Color _end;

	void update() override {
		auto obj = _object.lock();
		if (!obj) { stop(); return; }

		if (!_initialized) {
			_start = obj->getTriangleColor(0);
			_initialized = true;
		}

		float t = progress();
		Color current(
			static_cast<uint8_t>(_start.r() * (1 - t) + _end.r() * t),
			static_cast<uint8_t>(_start.g() * (1 - t) + _end.g() * t),
			static_cast<uint8_t>(_start.b() * (1 - t) + _end.b() * t),
			static_cast<uint8_t>(_start.a() * (1 - t) + _end.a() * t)
		);
		obj->applyColorToAllTriangles(current);
	}
public:
	template <typename... Args>
	SetColorAnim(std::shared_ptr<MeshComponent> mesh, Color value, Args&&... args)
		: Animation(std::forward<Args>(args)...), _object(mesh), _end(value) {
	}
};

#endif