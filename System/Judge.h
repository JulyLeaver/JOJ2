#pragma once

#include <map>
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

	CompileIdle,
	Compiling,

	CompileError,
	ExeIdle,
	Executing,
	AC,
	WA,
	TLE,
	MLE,
	RTE,

	WRAP_UP
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

public:
	Judge(Logger* logger);
	~Judge();

	void judge(Mark* mark);
	void shutdown();
};