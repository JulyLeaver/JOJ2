#pragma once

#include <iostream>
#include <string>
#include "Event.h"
#include "Mark.h"

using std::string;

class Logger : public Event<string>
{
private:
	FILE* fStream;

public:
	Logger(FILE* fStream);
	virtual ~Logger();

	inline void log(const string& s)
	{
		Event<string>::push(s);
	}
	inline void log(const Mark* mark, const string& msg)
	{
		const string s = '[' + mark->userId + ", " + std::to_string(mark->judge_number) + "] " + msg;
		Event<string>::push(s);
	}

private:
	void loop(std::unique_lock<std::mutex>& ul) override;
};