#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

class GameObject;

class Component
{
private:
	std::weak_ptr<GameObject> _object;
public:
	virtual ~Component() = default;

	virtual void start() {}
	virtual void update() {}

	void setObject(const std::shared_ptr<GameObject>& object) noexcept { _object = object; }
	std::shared_ptr<GameObject> getObject() const noexcept {
		if (auto locked = _object.lock()) return locked;
		return nullptr;
	}
};

#endif // !COMPONENT_H
