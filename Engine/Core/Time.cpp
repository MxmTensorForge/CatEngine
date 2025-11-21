#include "Time.h"

#include <SDL3/SDL.h>

float Time::_lastTime = 0;
float Time::_time = 0;
float Time::_deltaTime = 0;
bool Time::_firstFrame = true;

std::unordered_map<std::string, Time::Record> Time::_records;

void Time::update() noexcept {
	_time = static_cast<float>(SDL_GetPerformanceCounter()) / static_cast<float>(SDL_GetPerformanceFrequency());
	if (!_firstFrame) _deltaTime = _time - _lastTime;
	else {
		_firstFrame = false;
		_deltaTime = 0.0f;
	}

	_lastTime = _time;
}

float Time::time() noexcept {
	return _time;
}
float Time::deltaTime() noexcept {
	return _deltaTime;
}
float Time::fixedDeltaTime() noexcept {
	return _fixedDeltaTime;
}

void Time::begin(const std::string& tag) {
	auto& rec = _records[tag];
	rec.start = Clock::now();
	rec.running = true;
}
void Time::end(const std::string& tag) {
	auto it = _records.find(tag);
	if (it == _records.end() || !it->second.running) return;

	auto now = Clock::now();
	it->second.duration = std::chrono::duration<double, std::milli>(now - it->second.start).count();
	it->second.running = false;
}

double Time::get(const std::string& tag) {
	auto it = _records.find(tag);
	if (it == _records.end()) return 0.0;
	return it->second.duration;
}