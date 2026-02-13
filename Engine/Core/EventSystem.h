#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include <string>
#include <vector>

class EventSystem final
{
private:
	std::vector<std::string> _events;

	EventSystem() = default;
	~EventSystem() = default;
public:
	EventSystem(const EventSystem&) = delete;
	EventSystem& operator=(const EventSystem&) = delete;
	EventSystem(EventSystem&&) = delete;
	EventSystem& operator=(EventSystem&&) = delete;

	static EventSystem& getInstance() noexcept;

	void broadcast(const std::string& event);
	bool poll(const std::string& event);
};

#endif // !EVENTSYSTEM_H
