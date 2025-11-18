#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

enum class LogType
{
	Message,
	Warning,
	Error,
	Fatal
};

class Logger final
{
private:
	std::ofstream _file;

	std::string logType2Str(LogType type) const;
	std::string getTimestep() const;
public:
	static Logger& getInstance();

	void setLogFile(const std::string& name);
	void log(LogType type, const std::string& info);
};

#endif // !LOGGER_H
