#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <unordered_map>
#include <deque>
#include <string>
#include <memory>

#include "Animations/Animation.h"

using AnimTag = std::string;

class Animator final
{
private:
	static std::unordered_map<AnimTag, std::deque<std::unique_ptr<Animation>>> _animationList;
	static void update();

	friend class Application;
public:
	template <typename T, typename... Args>
	static void add(const AnimTag& tag, Args&&... args) {
		_animationList[tag].emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
	}
	static void remove(AnimTag tag);
};

#endif // !ANIMATOR_H