#include "EventSystem.h"

EventSystem& EventSystem::getInstance() noexcept {
	static EventSystem es;
	return es;
}

void EventSystem::broadcast(const std::string& event) {
	if (std::find(_events.begin(), _events.end(), event) == _events.end()) {
		_events.push_back(event);
	}
}
bool EventSystem::poll(const std::string& event) {
	auto e = std::find(_events.begin(), _events.end(), event);
	if (e == _events.end()) return false;

	_events.erase(e);
	return true;
}