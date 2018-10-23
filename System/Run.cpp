#include <cstring>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/user.h>
#include "Judge.h"
#include "Run.h"

Run::Run(Logger* logger, WrapUp* wp) : logger(logger), wp(wp)
{
	CCU = 2;

	user_out = new char[OUT_MAX];
	ac_out = new char[OUT_MAX];
}

Run::~Run()
{
	delete user_out;
	delete ac_out;
}

void Run::loop(std::unique_lock<std::mutex>& ul)
{
	// 무조건 lock 상태에서 탈출
	while (true)
	{
		ul.lock();
		if (_q.empty()) break;

		std::vector<std::thread> executing;
		std::vector<Mark*> marks;

		for (int i = 0; i < CCU; ++i) // 동시 접속 중일 때... 즉 한명 접속 하고 그 한명을 위한 채점 중일 떄 다른 한명이 push되어도 대기된다.
		{
			if (_q.empty()) break;

			Mark* mark = _q.front(); _q.pop();

			executing.emplace_back(&Run::executing, this, mark);
			marks.emplace_back(mark);
		}

		ul.unlock();

		for (int i = 0; i < (int)executing.size(); ++i) executing[i].join(); // 즉, 여기 바로 오기전 까지 동접이 없다면 그 한명을 위한 채점 중일 때 다른 한명이 push 되어도 대기
		for (auto i : marks) wp->wrapUp(i);
	}
}

void Run::executing(Mark* mark)
{
	mark->state = State::Executing;

	logger->log(mark, "execut start");

	const int LIM_TIME_SEC = mark->LIM_TIME_SEC;
	const int LIM_MEM_MB = mark->LIM_MEM_MB;

	struct rlimit rlim;
	rlim.rlim_cur = LIM_TIME_SEC;
	rlim.rlim_max = LIM_TIME_SEC;

	const int TC_NUM = mark->TC_NUM;
	int now_tc_num;
	for (now_tc_num = 1; now_tc_num <= TC_NUM; ++now_tc_num)
	{
		logger->log(mark, std::to_string(now_tc_num) + " TC excuting");

		int pid = fork();
		if (pid == 0)
		{
			freopen(mark->execute_stdout_file_path.c_str(), "wt", stdout); // 표준 출력
			const string tc_stdin = mark->problemNumPath + std::to_string(now_tc_num) + ".in"; // ../Problems/9999/i.in
			freopen(tc_stdin.c_str(), "rt", stdin); // 테스트 케이스 입력 

			setrlimit(RLIMIT_CPU, &rlim);

			ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
			const string exe_string = "./" + mark->binFilePath; // 실행 경로
			execl(exe_string.c_str(), exe_string.c_str(), nullptr);
			exit(0);
		}
		int status;
		struct rusage ruse;
		struct user_regs_struct reg;
		while (true)
		{
			wait4(pid, &status, 0, &ruse);
			ptrace(PTRACE_GETREGS, pid, nullptr, &reg);

			// 정상 종료
			if (WIFEXITED(status))
			{
				logger->log(mark, std::to_string(now_tc_num) + " TC normal shutdown");

				if (WEXITSTATUS(status) != 0)
				{
					mark->state = State::RTE;
					break;
				}

				logger->log(mark, std::to_string(now_tc_num) + " TC grading start");

				const string tc_stdout = mark->problemNumPath + std::to_string(now_tc_num) + ".out";
				if (!grading(mark->execute_stdout_file_path.c_str(), tc_stdout))
				{
					mark->state = State::WA;
				}

				logger->log(mark, std::to_string(now_tc_num) + " TC grading end");

				break;
			}

			// 시그널 발생
			if (WIFSIGNALED(status))
			{
				switch (WTERMSIG(status))
				{
				case SIGKILL:
				case SIGXCPU:
					mark->state = State::TLE;
					break;
				default:
					mark->state = State::RTE;
					break;
				}
			//	ptrace(PTRACE_KILL, pid, nullptr, nullptr);
				break;
			}

			ptrace(PTRACE_SYSCALL, pid, nullptr, nullptr);
		}

		if (mark->state != State::Executing) break;
		++(mark->AC_NUM);
	}
	if (TC_NUM < now_tc_num) mark->state = State::AC;

	logger->log(mark, "execute end");
}

bool Run::grading(const string& user_out_file_path, const string& ac_out_file_path)
{
	FILE* user_out_dataF = fopen(user_out_file_path.c_str(), "rt");
	FILE* ac_out_dataF = fopen(ac_out_file_path.c_str(), "rt");
	bool ret = true;

	bool userEnd = false;
	while (true)
	{
		memset(ac_out, 0, sizeof(OUT_MAX));
		fgets(ac_out, OUT_MAX, ac_out_dataF);

		memset(user_out, 0, sizeof(OUT_MAX));
		fgets(user_out, OUT_MAX, user_out_dataF);

		int user_eof = feof(user_out_dataF), ac_eof = feof(ac_out_dataF);

		if (!cmp(ac_out, user_out))
		{
			ret = false;
			break;
		}

		if (user_eof && ac_eof) break;
	}

	fclose(user_out_dataF);
	fclose(ac_out_dataF);
	return ret;
}

bool Run::cmp(const char* ac, const char* user)
{
	int acS = strlen(ac), userS = strlen(user);
	
	for (--acS; 0 <= acS; --acS) if ((ac[acS] != ' ') && (ac[acS] != '\n') && (ac[acS] != '\0')) break;
	for (--userS; 0 <= userS; --userS) if ((user[userS] != ' ') && (user[userS] != '\n') && (user[userS] != '\0')) break;

	if (acS != userS) return false;

	// 널문자도 제거 하므로 닫힌 구간
	for (int i = 0; i <= acS; ++i) if (ac[i] != user[i]) return false;

	return true;
}

