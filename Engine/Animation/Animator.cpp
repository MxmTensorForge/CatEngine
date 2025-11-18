#include "Animator.h"
#include "../Core/Logger.h"

std::unordered_map<AnimTag, std::deque<std::unique_ptr<Animation>>> Animator::_animationList;

void Animator::update() {
    for (auto& [tag, deque] : _animationList)
    {
        while (!deque.empty()) {
            if (!deque.front()->updateState()) deque.pop_front();
            else break;
        }
    }

    auto it = _animationList.begin();
    while (it != _animationList.end()) {
        if (it->second.empty()) {
            it = _animationList.erase(it);
        }
        else {
            it++;
        }
    }
}

void Animator::remove(AnimTag tag) {
    auto it = _animationList.find(tag);

    if (it != _animationList.end()) {
        _animationList.erase(it);
    }
    else {
        Logger::getInstance().log(LogType::Warning, "animation not found");
    }
}