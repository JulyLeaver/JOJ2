#pragma once

#include <string>

class Logger;
class Mark;
class Compile;
class Run;
class WrapUp;

/*
CompileIdle -> Compiling -> ExeIdle                    -> Executing -> (AC, WA, TLE, MLE, RTE) -> WrapUp
						 -> CompileError -> WrapUp
*/
enum class State
{
	//	None,
	//	Idle,

	CompileIdle = 0,
	Compiling,

	CompileError,
	ExeIdle,
	Executing,
	AC,
	WA,
	TLE,
	MLE,
	RTE,

	WRAP_UP,

	CNT
};

/*
Compile -> Run -> Grade
*/
class Judge
{
private:
	Logger* logger;
	Compile* compile;
	Run* run;
	WrapUp* wp;
	int judge_number;

private:
	static const char* stringString[static_cast<int>(State::CNT)];

public:
	Judge(Logger* logger);
	~Judge();

	void judge(const std::string& userId, const std::string& srcFileName, const int problemNum);
	void judge(Mark* mark);

	static std::string state2string(State state)
	{
		return stringString[static_cast<int>(state)];
	}

	void shutdown();
};