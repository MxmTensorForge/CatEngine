#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <memory>
#include <type_traits>
#include <string>

#include "../Component.h"
#include "Transform.h"

class Scene;

class GameObject final
{
private:
	std::vector<std::unique_ptr<Component>> _components;
    Transform _transform{};

    std::string _name;
    std::string _tag;
    bool _isActive = true;
public:
    explicit GameObject(const std::string& name, const std::string& tag = "default") : _name(name), _tag(tag) { _transform.setOwner(this); }

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    const std::string& getName() const noexcept { return _name; }
    void setName(const std::string& name) noexcept { _name = name; }
    
    const std::string& getTag() const noexcept { return _tag; }
    void setTag(const std::string& tag) noexcept { _tag = tag; }

    inline Transform& transform() noexcept { return _transform; }
    inline const Transform& transform() const noexcept { return _transform; }

    template <typename T>
    T* getComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T is not component");

        for (const auto& c : _components) {
            if (T* result = dynamic_cast<T*>(c.get())) {
                return result;
            }
        }
        return nullptr;
    }

    template <typename T>
    const T* getComponent() const {
        static_assert(std::is_base_of<Component, T>::value, "T is not component");

        for (const auto& c : _components) {
            if (const T* result = dynamic_cast<const T*>(c.get())) {
                return result;
            }
        }
        return nullptr;
    }

    template <typename T>
    bool hasComponent() const {
        static_assert(std::is_base_of<Component, T>::value, "T is not component");

        return getComponent<T>() != nullptr;
    }

    template <typename T, typename... Args>
    T* addComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T is not component");

        _components.emplace_back(
            std::make_unique<T>(std::forward<Args>(args)...)
        );

        T* raw = static_cast<T*>(_components.back().get());
        raw->setObject(this);

        

        return raw;
    }

    template <typename T>
    void removeComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T is not component");

        _components.erase(
            std::remove_if(_components.begin(), _components.end(),
                [](const std::unique_ptr<Component>& comp) {
                    return dynamic_cast<T*>(comp.get()) != nullptr;
                }), 
            _components.end()
        );
    }

    inline void startComponents() {
        for (auto& c : _components) {
            c->start();
        }
    }
    inline void updateComponents() {
        for (auto& c : _components) {
            c->update();
        }
    }

    inline void setActive(bool active) noexcept {
        _isActive = active;
    }
    inline bool getActive() const noexcept {
        return _isActive;
    }
};

#endif // !GAMEOBJECT_H