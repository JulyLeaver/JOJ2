#include "Logger.h"

Logger::Logger(FILE* fStream) : fStream(fStream)
{
}

Logger::~Logger()
{
}

void Logger::loop(std::unique_lock<std::mutex>& ul)
{
	while (true)
	{
		ul.lock();
		if (_q.empty()) break;
		std::cout << _q.front() << '\n';
		_q.pop();
		ul.unlock();
	}
}
