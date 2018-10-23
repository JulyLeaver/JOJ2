#include "Judge.h"
#include "Compile.h"
#include "Run.h"
#include "WrapUp.h"

const char* Judge::stringString[static_cast<int>(State::CNT)] = {
		"CompileIdle",
		"Compiling",

		"CompileError",
		"ExeIdle",
		"Executing",
		"AC",
		"WA",
		"TLE",
		"MLE",
		"RTE",

		"WRAP_UP"
};

Judge::Judge(Logger* logger) : logger(logger)
{
	wp = new WrapUp(logger);
	run = new Run(logger, wp);
	compile = new Compile(logger, run, wp);

	judge_number = 0;
}

Judge::~Judge()
{
}

void Judge::judge(const std::string& userId, const std::string& srcFileName, const int problemNum)
{
	judge(new Mark(userId, srcFileName, problemNum, ++(this->judge_number)));
}

void Judge::judge(Mark* mark)
{
	compile->compile(mark);
}

void Judge::shutdown()
{
	compile->end();
	run->end();
	wp->end();

	logger->log("Judge shutdown");
}
