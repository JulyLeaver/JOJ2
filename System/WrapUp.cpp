#include "WrapUp.h"

WrapUp::WrapUp(Logger* logger) : logger(logger)
{
}

WrapUp::~WrapUp()
{
}

void WrapUp::loop(std::unique_lock<std::mutex>& ul)
{
	while (true)
	{
		ul.lock();
		if (_q.empty()) break;
		Mark* mark = _q.front(); _q.pop();
		ul.unlock();

		logger->log('[' + mark->userId + "] end");
		delete mark;
	}
}
