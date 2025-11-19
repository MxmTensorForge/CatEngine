#ifndef TIME_H
#define TIME_H

#include <chrono>
#include <unordered_map>
#include <string>

class Time final
{
public:
	Time() = delete;
	~Time() = delete;

	static float time() noexcept;
	static float deltaTime() noexcept;

	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<Clock>;

	struct Record
	{
		TimePoint start;
		double duration = 0.0;
		bool running = false;
	};

	static void begin(const std::string& tag);
	static void end(const std::string& tag);

	static double get(const std::string& tag);
private:
	static float _deltaTime;
	static float _time;
	static float _lastTime;
	static bool _firstFrame;

	static std::unordered_map<std::string, Record> _records;

	static void update() noexcept;

	friend class Application;
};

#endif