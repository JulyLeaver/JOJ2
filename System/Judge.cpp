#include "Judge.h"
#include "Compile.h"
#include "Run.h"
#include "WrapUp.h"

Judge::Judge(Logger* logger) : logger(logger)
{
	wp = new WrapUp(logger);
	run = new Run(logger, wp);
	compile = new Compile(logger, run, wp);
}

Judge::~Judge()
{
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
}
