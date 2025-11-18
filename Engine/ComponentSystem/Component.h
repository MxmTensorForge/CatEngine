#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

class GameObject;

class Component
{
private:
	std::shared_ptr<GameObject> _object = nullptr;
public:
	virtual ~Component() = default;

	virtual void start() {}
	virtual void update() {}

	void setObject(const std::shared_ptr<GameObject>& object) noexcept { _object = object; }
	std::shared_ptr<GameObject> getObject() const noexcept {
		return _object;
	}
};

#endif // !COMPONENT_H
