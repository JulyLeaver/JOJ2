#pragma once

#include <iostream>
#include "Event.h"
#include "Mark.h"
#include "Logger.h"

class WrapUp : public Event<Mark*>
{
private:

	Logger* logger;
public:
	WrapUp(Logger* logger);
	virtual ~WrapUp();

	inline void wrapUp(Mark* mark)
	{
		Event<Mark*>::push(mark);
	}
	virtual void loop(std::unique_lock<std::mutex>& ul);
};