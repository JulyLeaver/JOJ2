#include "WrapUp.h"
#include "Judge.h"

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

		logger->log(mark, "end, result = " + Judge::state2string(mark->state));

		delete mark;
	}
}
