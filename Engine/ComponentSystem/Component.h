#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

class GameObject;

class Component
{
private:
	GameObject* _object;
public:
	virtual ~Component() = default;

	virtual void start() {}
	virtual void update() {}

	void setObject(GameObject* object) noexcept { _object = object; }
	GameObject* getObject() const noexcept {
		return _object;
	}
};

#endif // !COMPONENT_H
