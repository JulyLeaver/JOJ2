#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Compile.h"
#include "Judge.h"

Compile::Compile(Logger* logger, Run* run, WrapUp* wp) : logger(logger), run(run), wp(wp)
{
}

Compile::~Compile()
{
}

void Compile::loop(std::unique_lock<std::mutex>& ul)
{
	while (true)
	{
		ul.lock();
		if (_q.empty()) break;
		const int waiting_cnt = _q.size();
		Mark* mark = _q.front(); _q.pop();
		ul.unlock();

		mark->state = State::Compiling;
		
		logger->log(mark, "compiling");

		int pid = fork();
		if (pid == 0)
		{
			freopen(mark->compile_stderr_file_path.c_str(), "wt", stderr);
			execvp(mark->compileCmd[0], (char** const)mark->compileCmd);
			exit(0);
		}
		int status;
		wait(&status);

		if (WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0) mark->state = State::CompileError;
		else mark->state = State::ExeIdle;

		logger->log(mark, string("compile ") + (mark->state == State::ExeIdle ? "success" : "fail"));

		if (mark->state == State::ExeIdle) run->run(mark);
		else wp->wrapUp(mark);
	}
}