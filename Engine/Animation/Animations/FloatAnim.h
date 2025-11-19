#ifndef FLOATANIM_H
#define FLOATANIM_H

#include "Animation.h"
#include <functional>
#include <memory>

class FloatAnim final : public Animation
{
private:
    std::function<void(float)> _setter;
    float _start;
    float _end;

    void update() override {
        float t = progress();
        float current = _start + (_end - _start) * t;
        _setter(current);
    }

public:
    template <typename... Args>
    FloatAnim(float startValue, float endValue, const std::function<void(float)>& setter, Args&&... args)
        : Animation(std::forward<Args>(args)...), _start(startValue), _end(endValue), _setter(setter) {
    }
};

#endif