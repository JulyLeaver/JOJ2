#pragma once

#include "Event.h"
#include "Mark.h"
#include "Logger.h"
#include "Run.h"
#include "WrapUp.h"

class Compile : public Event<Mark*>
{
private:
	Logger* logger;
	Run* run;
	WrapUp* wp;
public:
	Compile(Logger* logger, Run* run, WrapUp* wp);
	virtual ~Compile();

	inline void compile(Mark* mark)
	{
		Event<Mark*>::push(mark);
	}
	virtual void loop(std::unique_lock<std::mutex>& ul);
};

