#include <iostream>
#include "Mark.h"
#include "Judge.h"

const char* CONDITION_FILE_NAME = "condi";	// 문제 조건 파일 이름
const char* EXECUTE_STDOUT_FILE_EXTENSION = ".stdout";
const char* COMPILE_STDERR_FILE_EXTENSION = ".compile.stderr";
const char* USER_DIR_PATH = "../Users/";
const char* PROBLEMS_DIR_PATH = "../Problems/";

Mark::Mark(const string& userId, const string& srcFileName, const int problemNum, const int judge_number) :
	userId(userId), srcFileName(srcFileName), problemNum(problemNum), judge_number(judge_number),
	srcFilePath(USER_DIR_PATH + userId + "/" + srcFileName),
	binFilePath(USER_DIR_PATH + userId + "/" + srcFileName + ".bin"),
	/*
	proInputPath(PRO_FOR_PATH + std::to_string(proNum) + ".in"),
	acDataPath(PRO_FOR_PATH + std::to_string(proNum) + ".out"),
	*/
	problemNumPath(PROBLEMS_DIR_PATH + std::to_string(problemNum) + "/"),
	execute_stdout_file_path(USER_DIR_PATH + userId + "/" + srcFileName + EXECUTE_STDOUT_FILE_EXTENSION),
	compile_stderr_file_path(USER_DIR_PATH + userId + "/" + srcFileName + COMPILE_STDERR_FILE_EXTENSION),
	state(State::CompileIdle)
{
	int dotIdx = -1;
	for (int i = (int)srcFileName.size() - 1; 0 <= i; --i)
	{
		if (srcFileName[i] == '.')
		{
			dotIdx = i;
			break;
		}
	}
	const string& extension = srcFileName.substr(dotIdx + 1);
	if (extension == "c")
	{
		compileCmd = new const char*[6]{
			"gcc", srcFilePath.c_str(), "-o", binFilePath.c_str(), "-DONLINE_JUDGE", nullptr
		};
	}
	else if (extension == "cpp")
	{
		compileCmd = new const char*[8]{
			"g++", srcFilePath.c_str(), "-o", binFilePath.c_str(), "-std=c++11", "-DONLINE_JUDGE", nullptr
		};
	}
	else
	{
		support = false;
		msg = "extension unsupported";
		return;
	}

	const string conditionPath = problemNumPath + CONDITION_FILE_NAME;
	FILE* conFile = fopen(conditionPath.c_str(), "rt");
	if (conFile == nullptr)
	{
		support = false;
		msg = conditionPath + " file open fail";
		return;
	}
	fscanf(conFile, "%d %d %d", &TC_NUM, &LIM_TIME_SEC, &LIM_MEM_MB);
	fclose(conFile);

	LIM_TIME_SEC /= 1000;
	++LIM_TIME_SEC;
	LIM_MEM_MB += 20;

	USE_TIME_MS = -1;
	USE_MEM_MB = -1;
}

Mark::~Mark()
{
	delete[]compileCmd;
}